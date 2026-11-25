/*
 * produto.h
 * Lista duplamente encadeada de produtos.
 * Responsavel: Miguel.
 */
#ifndef PRODUTO_H
#define PRODUTO_H

#include "tipos.h"

/* Descritor da lista: guarda o primeiro e o ultimo no.
 * Guardar o 'fim' permite inserir no final sem percorrer a lista. */
typedef struct ListaProdutos {
    Produto *inicio;     /* primeiro no (NULL se a lista esta vazia) */
    Produto *fim;        /* ultimo no (NULL se a lista esta vazia) */
    int quantidade;      /* numero de produtos cadastrados */
} ListaProdutos;

/* Deixa a lista vazia. Deve ser chamada antes de qualquer outra funcao.
 * Recebe: ponteiro para a lista. Retorna: nada. */
void inicializarListaProdutos(ListaProdutos *lista);

/* Adiciona um produto ao FIM da lista.
 * Recebe: lista, id, nome e valor do produto.
 * Retorna: 1  = sucesso;
 *          0  = dados invalidos (id <= 0, nome vazio, valor < 0) ou id ja existente;
 *          -1 = sem memoria. */
int inserirProduto(ListaProdutos *lista, int id, const char *nome, double valor);

/* Busca um produto pelo id.
 * Recebe: lista e id procurado.
 * Retorna: ponteiro para o produto, ou NULL se nao existir. */
Produto *buscarProdutoPorId(const ListaProdutos *lista, int id);

/* Imprime todos os produtos, do inicio ao fim, em colunas alinhadas.
 * Se a lista estiver vazia, informa isso ao usuario. */
void listarProdutos(const ListaProdutos *lista);

/* Libera toda a memoria da lista e a deixa vazia (pronta para reuso).
 * Deve ser chamada ao encerrar o programa. */
void liberarListaProdutos(ListaProdutos *lista);

#endif