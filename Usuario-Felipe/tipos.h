/*
 * tipos.h
 * Structs compartilhadas por todo o projeto (Sistema de Logistica de Cargas).
 * Responsavel: Miguel.
 *
 * ATENCAO: todos os modulos dependem deste arquivo.
 * Nao altere sem avisar o grupo.
 */
#ifndef TIPOS_H
#define TIPOS_H

/* ---------- Constantes ---------- */
#define TAM_NOME           100  /* tamanho maximo de nomes (inclui o '\0') */
#define TAM_CPF            12   /* 11 digitos + '\0' */
#define TAM_DATA           11   /* "DD/MM/AAAA" + '\0' */
#define MAX_ENTREGAS_CARGA 50   /* limite de entregas por carga */

/* ---------- Produto (no da lista duplamente encadeada - Miguel) ---------- */
typedef struct Produto {
    int id;                      /* identificador unico do produto */
    char nome[TAM_NOME];         /* nome do produto */
    double valor;                /* preco em reais */
    struct Produto *anterior;    /* ponteiro para o no anterior (NULL se for o primeiro) */
    struct Produto *proximo;     /* ponteiro para o proximo no (NULL se for o ultimo) */
} Produto;

/* ---------- Entrega (item da fila - Pedro) ---------- */
typedef struct Entrega {
    int idProduto;                       /* id do produto a ser entregue */
    char cpfDestinatario[TAM_CPF];       /* CPF: 11 digitos numericos */
    char nomeDestinatario[TAM_NOME];     /* nome de quem recebe */
} Entrega;

/* ---------- Carga (no da arvore binaria de busca - Vinicius) ---------- */
typedef struct Carga {
    int idCarga;                              /* chave de ordenacao da arvore */
    char diaSaidaCaminhao[TAM_DATA];          /* formato "DD/MM/AAAA" */
    Entrega entregas[MAX_ENTREGAS_CARGA];     /* lista de entregas (vetor) */
    int qtdEntregas;                          /* quantas entregas ja estao na carga */
    struct Carga *esquerda;                   /* subarvore com ids menores */
    struct Carga *direita;                    /* subarvore com ids maiores */
} Carga;

#endif