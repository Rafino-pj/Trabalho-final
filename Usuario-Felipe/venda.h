/*
 * venda.h
 * Venda de produto e calculo de carga.
 * Responsavel: Iago.
 */
#ifndef VENDA_H
#define VENDA_H

#include "tipos.h"
#include "produto.h"

/* Fluxo da opcao 4 do menu (vender produto e mostrar a carga).
 * Recebe: lista de produtos e raiz da arvore de cargas.
 * Faz: 1) pede o id do produto e busca na lista;
 *      2) se nao existir, avisa e retorna;
 *      3) pede a data de entrega desejada (DD/MM/AAAA);
 *      4) percorre a arvore procurando cargas com dia de saida compativel
 *         (mesma data) e que ainda tenham espaco para mais entregas;
 *      5) mostra as cargas onde o produto pode entrar, ou avisa que nao ha
 *         carga disponivel.
 * Retorna: numero de cargas compativeis encontradas (0 se nenhuma),
 *          ou -1 se o produto nao existe, a entrada e invalida ou ha
 *          algum ponteiro nulo. */
int venderProduto(const ListaProdutos *lista, const Carga *raiz);

#endif
