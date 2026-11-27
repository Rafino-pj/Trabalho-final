/*
- arvore.h
- Arvore binaria de busca de cargas ordenada por idCarga.
- Responsavel: Vinicius.
 */
#ifndef ARVORE_H
#define ARVORE_H

#include "tipos.h"

int inserirCarga(Carga **raiz, Carga novaCarga);
Carga *buscarCarga(Carga *raiz, int idCarga);
void listarCargas(const Carga *raiz);
int buscarCargasPorData(const Carga *raiz, const char *diaSaidaCaminhao);
void liberarArvore(Carga **raiz);

#endif