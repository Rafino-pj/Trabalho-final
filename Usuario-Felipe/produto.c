/*
 * produto.c
 * Implementacao da lista duplamente encadeada de produtos.
 * Responsavel: Miguel.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"

/* ------------------------------------------------------------------
 * inicializarListaProdutos
 * Recebe: ponteiro para a lista.
 * Faz: deixa a lista vazia (inicio e fim NULL, quantidade 0).
 * ------------------------------------------------------------------ */
void inicializarListaProdutos(ListaProdutos *lista) {
    if (lista == NULL) return;   /* protecao contra ponteiro nulo */
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;
}

/* ------------------------------------------------------------------
 * inserirProduto
 * Recebe: lista, id, nome e valor.
 * Faz: valida os dados, rejeita id duplicado e insere o produto no FIM.
 * Retorna: 1 sucesso; 0 dados invalidos/id duplicado; -1 sem memoria.
 * ------------------------------------------------------------------ */
int inserirProduto(ListaProdutos *lista, int id, const char *nome, double valor) {
    Produto *novo;

    /* 1. Validacoes de entrada */
    if (lista == NULL || nome == NULL || nome[0] == '\0') return 0;
    if (id <= 0 || valor < 0) return 0;

    /* Nao permite id duplicado (antes do malloc, para nao alocar a toa) */
    if (buscarProdutoPorId(lista, id) != NULL) return 0;

    /* 2. Cria o novo no */
    novo = (Produto *) malloc(sizeof(Produto));
    if (novo == NULL) return -1;

    /* 3. Preenche os dados */
    novo->id = id;
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';   /* garante o terminador da string */
    novo->valor = valor;
    novo->proximo = NULL;              /* sera o ultimo, entao nao tem proximo */

    /* 4. Encaixa o no no fim da lista */
    if (lista->inicio == NULL) {
        /* Caso A: lista vazia -> o novo no e o primeiro e o ultimo */
        novo->anterior = NULL;
        lista->inicio = novo;
        lista->fim = novo;
    } else {
        /* Caso B: ja existem nos -> encaixa depois do ultimo (ordem importa!) */
        novo->anterior = lista->fim;   /* 1) novo aponta de volta para o antigo ultimo */
        lista->fim->proximo = novo;    /* 2) antigo ultimo aponta para o novo */
        lista->fim = novo;             /* 3) o fim da lista passa a ser o novo */
    }

    lista->quantidade++;
    return 1;
}

/* ------------------------------------------------------------------
 * buscarProdutoPorId
 * Recebe: lista e id procurado.
 * Faz: percorre a lista do inicio ao fim comparando os ids.
 * Retorna: ponteiro para o produto encontrado, ou NULL se nao existir.
 * ------------------------------------------------------------------ */
Produto *buscarProdutoPorId(const ListaProdutos *lista, int id) {
    Produto *atual;
    if (lista == NULL) return NULL;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        if (atual->id == id) return atual;   /* achou */
    }
    return NULL;                             /* chegou ao fim sem achar */
}

/* ------------------------------------------------------------------
 * listarProdutos
 * Recebe: lista.
 * Faz: imprime uma tabela com todos os produtos, do inicio ao fim.
 * ------------------------------------------------------------------ */
void listarProdutos(const ListaProdutos *lista) {
    Produto *atual;

    /* Caso de borda: lista vazia */
    if (lista == NULL || lista->inicio == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    /* Cabecalho da tabela */
    printf("%-6s | %-30s | %12s\n", "ID", "NOME", "VALOR (R$)");
    printf("-------+--------------------------------+-------------\n");

    /* Uma linha por produto */
    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        printf("%-6d | %-30s | %12.2f\n", atual->id, atual->nome, atual->valor);
    }
}

/* ------------------------------------------------------------------
 * liberarListaProdutos
 * Recebe: lista.
 * Faz: libera (free) todos os nos e deixa a lista vazia.
 * ------------------------------------------------------------------ */
void liberarListaProdutos(ListaProdutos *lista) {
    Produto *atual, *proximo;
    if (lista == NULL) return;

    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;   /* guarda o proximo ANTES de liberar o atual */
        free(atual);                /* libera o no atual */
        atual = proximo;            /* avanca para o no guardado */
    }

    /* Deixa a lista consistente (sem ponteiros para memoria ja liberada) */
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;
}