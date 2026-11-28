/* main.c - menu principal do sistema */
#include <stdio.h>
#include <string.h>
#include "interface.h"
#include "tipos.h"
#include "produto.h"
#include "fila.h"
#include "arvore.h"
#include "venda.h"
#include "usuario.h"

/* Recebe: a tabela hash de usuarios.
 * Retorna: 1 se o acesso foi liberado, 0 caso contrario.
 * Faz: le login e senha e consulta a tabela (opcao 1). */
static int opcaoValidarUsuario(NoUsuario *tabela[TAM_TABELA])
{
    Usuario dados;

    imprimirCabecalho("VALIDACAO DE USUARIO");
    if (!lerString("Login: ", dados.login, sizeof dados.login) ||
        !lerString("Senha: ", dados.senha, sizeof dados.senha)) {
        return 0;
    }
    if (dados.login[0] == '\0' || dados.senha[0] == '\0') {
        printf("Login e senha nao podem ficar vazios.\n");
        return 0;
    }
    if (!validarUsuario(&dados, tabela)) {
        printf("Login ou senha incorretos.\n");
        return 0;
    }
    printf("Acesso liberado.\n");
    return 1;
}

/* Recebe: a lista de produtos.
 * Retorna: nada.
 * Faz: le os dados e insere o produto na lista (opcao 2). */
static void opcaoCadastrarProduto(ListaProdutos *lista)
{
    int id, resultado;
    char nome[TAM_NOME];
    double valor;

    imprimirCabecalho("CADASTRO DE PRODUTOS");
    if (!lerInteiro("Id: ", &id) ||
        !lerString("Nome: ", nome, sizeof nome) ||
        !lerReal("Valor (R$): ", &valor)) {
        return;
    }

    resultado = inserirProduto(lista, id, nome, valor);
    if (resultado == 1) {
        printf("Produto cadastrado.\n");
    } else if (resultado == 0) {
        printf("Dados invalidos ou id ja cadastrado.\n");
    } else {
        printf("Sem memoria.\n");
    }
}

/* Recebe: a lista de produtos e a fila de entregas.
 * Retorna: nada.
 * Faz: le os dados de uma entrega e a coloca no fim da fila. */
static void cadastrarEntrega(const ListaProdutos *lista, Fila *fila)
{
    Entrega entrega;
    int resultado;

    memset(&entrega, 0, sizeof entrega);
    if (!lerInteiro("Id do produto: ", &entrega.idProduto)) {
        return;
    }
    if (buscarProdutoPorId(lista, entrega.idProduto) == NULL) {
        printf("Produto nao encontrado.\n");
        return;
    }
    if (!lerString("Nome do destinatario: ", entrega.nomeDestinatario,
                   sizeof entrega.nomeDestinatario) ||
        !lerString("CPF (11 digitos): ", entrega.cpfDestinatario,
                   sizeof entrega.cpfDestinatario)) {
        return;
    }
    if (entrega.nomeDestinatario[0] == '\0') {
        printf("O nome nao pode ficar vazio.\n");
        return;
    }

    resultado = enfileirarEntrega(fila, entrega);
    if (resultado == 1) {
        printf("Entrega adicionada a fila.\n");
    } else if (resultado == 0) {
        printf("Entrega nao cadastrada.\n");
    } else {
        printf("Sem memoria.\n");
    }
}

/* Recebe: a fila de entregas e a raiz da arvore de cargas.
 * Retorna: nada.
 * Faz: cria uma carga com as primeiras entregas da fila, ate o
 *      limite. Elas so saem da fila se a carga entrar na arvore. */
static void montarCarga(Fila *fila, Carga **raiz)
{
    Carga nova;
    const NoEntrega *no;
    int i, resultado;

    if (filaVazia(fila)) {
        printf("Fila vazia: cadastre entregas antes de montar a carga.\n");
        return;
    }

    memset(&nova, 0, sizeof nova);
    if (!lerInteiro("Id da carga: ", &nova.idCarga)) {
        return;
    }
    if (buscarCarga(*raiz, nova.idCarga) != NULL) {
        printf("Ja existe uma carga com esse id.\n");
        return;
    }
    if (!lerString("Dia de saida (DD/MM/AAAA): ", nova.diaSaidaCaminhao,
                   sizeof nova.diaSaidaCaminhao)) {
        return;
    }

    for (no = fila->inicio;
         no != NULL && nova.qtdEntregas < MAX_ENTREGAS_CARGA;
         no = no->proximo) {
        nova.entregas[nova.qtdEntregas++] = no->dados;
    }

    resultado = inserirCarga(raiz, nova);
    if (resultado == 1) {
        for (i = 0; i < nova.qtdEntregas; i++) {
            desenfileirarEntrega(fila);
        }
        printf("Carga montada com %d entrega(s).\n", nova.qtdEntregas);
    } else if (resultado == 0) {
        printf("Carga nao cadastrada: confira o id e a data.\n");
    } else {
        printf("Sem memoria.\n");
    }
}

/* Recebe: a lista, a fila e a raiz da arvore.
 * Retorna: nada.
 * Faz: mostra o submenu de entregas e cargas (opcao 3). */
static void opcaoEntregasECargas(const ListaProdutos *lista, Fila *fila,
                                 Carga **raiz)
{
    int opcao;

    imprimirCabecalho("ENTREGAS E CARGAS");
    printf("1 - Cadastrar entrega\n");
    printf("2 - Montar carga com as entregas da fila\n");
    printf("0 - Voltar\n");
    if (!lerInteiro("Escolha uma opcao: ", &opcao)) {
        return;
    }

    switch (opcao) {
    case 1: cadastrarEntrega(lista, fila); break;
    case 2: montarCarga(fila, raiz);       break;
    case 0:                                break;
    default: printf("Opcao inexistente.\n");
    }
}

/* Recebe: a lista de produtos e a raiz da arvore.
 * Retorna: nada.
 * Faz: chama a venda, modulo do Iago (opcao 4). */
static void opcaoVenderProduto(const ListaProdutos *lista, const Carga *raiz)
{
    imprimirCabecalho("VENDA DE PRODUTO");
    venderProduto(lista, raiz);
}

/* Recebe: os produtos, a fila de entregas e a arvore de cargas.
 * Retorna: nada.
 * Faz: imprime os tres relatorios (produtos, entregas pendentes e cargas). */
static void opcaoRelatorios(const ListaProdutos *lista, const Fila *fila, const Carga *raiz)
{
    imprimirCabecalho("RELATORIOS");
    listarProdutos(lista);
    listarFila(fila);
    listarCargas(raiz);
}

/* Recebe: nada.
 * Retorna: nada.
 * Faz: mostra as opcoes do menu principal. */
static void mostrarMenu(void)
{
    imprimirCabecalho("MENU PRINCIPAL");
    printf("1 - Validar usuario\n");
    printf("2 - Cadastrar produtos\n");
    printf("3 - Cadastrar entregas e montar cargas\n");
    printf("4 - Vender produto\n");
    printf("5 - Relatorios\n");
    printf("6 - Sair\n");
    imprimirSeparador();
}

/* Recebe: nada. Retorna: 0 ao encerrar.
 * Faz: prepara as estruturas, repete o menu ate Sair e libera a memoria. */
int main(void)
{
    ListaProdutos produtos;
    Fila entregas;
    Carga *cargas = NULL;
    NoUsuario *usuarios[TAM_TABELA] = { NULL };
    Usuario usuarioTeste = { "admin", "1234" }; /* apenas para testes */
    int opcao;
    int logado = 0;

    inicializarListaProdutos(&produtos);
    inicializarFila(&entregas);
    if (cadastrarUsuario(&usuarioTeste, usuarios) != 1) {
        printf("Nao foi possivel cadastrar o usuario inicial.\n");
    }

    imprimirTelaAbertura();

    do {
        mostrarMenu();
        if (!lerInteiro("Escolha uma opcao: ", &opcao)) {
            break; /* fim da entrada */
        }

        /* as opcoes 2 a 5 exigem usuario validado */
        if (opcao >= 2 && opcao <= 5 && !logado) {
            printf("Acesso negado: valide o usuario na opcao 1.\n");
            pausar();
            continue;
        }

        switch (opcao) {
        case 1: logado = opcaoValidarUsuario(usuarios);              break;
        case 2: opcaoCadastrarProduto(&produtos);                    break;
        case 3: opcaoEntregasECargas(&produtos, &entregas, &cargas); break;
        case 4: opcaoVenderProduto(&produtos, cargas);               break;
        case 5: opcaoRelatorios(&produtos, &entregas, cargas);                                   break;
        case 6:                                                      break;
        default: printf("Opcao inexistente.\n");
        }

        if (opcao != 6) {
            pausar();
        }
    } while (opcao != 6);

    liberarListaProdutos(&produtos);
    liberarFila(&entregas);
    liberarArvore(&cargas);

    printf("\nPrograma encerrado.\n");
    return 0;
}
