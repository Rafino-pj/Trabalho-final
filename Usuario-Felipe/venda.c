/*
 * venda.c
 * Implementacao da venda de produto e do calculo de carga.
 * Responsavel: Iago.
 */
#include <stdio.h>
#include <string.h>
#include "venda.h"

/* ------------------------------------------------------------------
 * lerLinha (funcao auxiliar, so deste arquivo)
 * Recebe: buffer e seu tamanho.
 * Faz: le uma linha com fgets e remove o '\n' do final.
 * Retorna: 1 se leu algo, 0 em caso de erro/fim de entrada.
 * ------------------------------------------------------------------ */
static int lerLinha(char *buffer, int tamanho) {
    size_t n;
    if (fgets(buffer, tamanho, stdin) == NULL) return 0;

    n = strlen(buffer);
    if (n > 0 && buffer[n - 1] == '\n') {
        buffer[n - 1] = '\0';
    } else {
        /* linha maior que o buffer: descarta o resto para nao sujar a proxima leitura */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    return 1;
}

/* ------------------------------------------------------------------
 * dataValida (funcao auxiliar)
 * Recebe: texto digitado pelo usuario.
 * Faz: confere apenas o formato "DD/MM/AAAA" (digitos e barras nos
 *      lugares certos) e se dia/mes estao em faixa razoavel.
 * Retorna: 1 se valida, 0 caso contrario.
 * ------------------------------------------------------------------ */
static int dataValida(const char *data) {
    int i, dia, mes;

    if (data == NULL || strlen(data) != 10) return 0;

    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') return 0;
        } else if (data[i] < '0' || data[i] > '9') {
            return 0;
        }
    }

    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    return dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12;
}

/* ------------------------------------------------------------------
 * mostrarCargasCompativeis (funcao auxiliar, recursiva)
 * Recebe: no atual da arvore e a data desejada.
 * Faz: percorre a arvore EM ORDEM (esquerda, no, direita), entao as
 *      cargas aparecem ordenadas por idCarga. Para cada carga com
 *      diaSaidaCaminhao igual a data e com vaga, imprime uma linha.
 * Retorna: quantidade de cargas compativeis encontradas nessa subarvore.
 * ------------------------------------------------------------------ */
static int mostrarCargasCompativeis(const Carga *no, const char *data) {
    int encontradas = 0;

    if (no == NULL) return 0;   /* caso base: subarvore vazia */

    encontradas += mostrarCargasCompativeis(no->esquerda, data);

    if (strcmp(no->diaSaidaCaminhao, data) == 0 &&
        no->qtdEntregas < MAX_ENTREGAS_CARGA) {
        printf("%-8d | %-12s | %2d/%d\n",
               no->idCarga, no->diaSaidaCaminhao,
               no->qtdEntregas, MAX_ENTREGAS_CARGA);
        encontradas++;
    }

    encontradas += mostrarCargasCompativeis(no->direita, data);

    return encontradas;
}

/* ------------------------------------------------------------------
 * venderProduto
 * Recebe: lista de produtos e raiz da arvore de cargas.
 * Faz: ver venda.h.
 * Retorna: numero de cargas compativeis; -1 em caso de erro.
 * ------------------------------------------------------------------ */
int venderProduto(const ListaProdutos *lista, const Carga *raiz) {
    char entrada[TAM_NOME];
    char data[TAM_DATA];
    int id, encontradas;
    Produto *produto;

    if (lista == NULL) return -1;

    /* 1. Pede e valida o id do produto */
    printf("Id do produto: ");
    if (!lerLinha(entrada, sizeof(entrada))) return -1;

    if (sscanf(entrada, "%d", &id) != 1 || id <= 0) {
        printf("Id invalido. Digite um numero inteiro positivo.\n");
        return -1;
    }

    /* 2. Busca o produto na lista */
    produto = buscarProdutoPorId(lista, id);
    if (produto == NULL) {
        printf("Produto com id %d nao encontrado.\n", id);
        return -1;
    }
    printf("Produto: %s | R$ %.2f\n", produto->nome, produto->valor);

    /* 3. Pede a data de entrega desejada */
    printf("Data de entrega desejada (DD/MM/AAAA): ");
    if (!lerLinha(data, sizeof(data))) return -1;

    if (!dataValida(data)) {
        printf("Data invalida. Use o formato DD/MM/AAAA.\n");
        return -1;
    }

    /* 4. Caso de borda: ainda nao ha cargas cadastradas */
    if (raiz == NULL) {
        printf("Nenhuma carga cadastrada. Nao ha carga disponivel.\n");
        return 0;
    }

    /* 5. Percorre a arvore e mostra as cargas compativeis */
    printf("\nCargas compativeis com a data %s:\n", data);
    printf("%-8s | %-12s | %s\n", "CARGA", "SAIDA", "ENTREGAS");
    printf("---------+--------------+---------\n");

    encontradas = mostrarCargasCompativeis(raiz, data);

    if (encontradas == 0) {
        /* o cabecalho ja foi impresso; avisa que nenhuma serve */
        printf("(nenhuma)\n");
        printf("Nao ha carga disponivel para a data %s.\n", data);
    } else {
        printf("\nO produto \"%s\" pode entrar em %d carga(s) acima.\n",
               produto->nome, encontradas);
    }

    return encontradas;
}
