#ifndef FILA_H
#define FILA_H

#include "tipos.h"

typedef struct NoEntrega
{
    Entrega dados;
    struct NoEntrega *proximo;
} NoEntrega;

typedef struct Fila
{
    NoEntrega *inicio;
    NoEntrega *fim;
} Fila;

void inicializarFila(Fila *fila);

int filaVazia(const Fila *fila);

int validarCPF(const char *cpf);

int enfileirarEntrega(Fila *fila, Entrega entrega);

Entrega desenfileirarEntrega(Fila *fila);

void listarFila(const Fila *fila);

void liberarFila(Fila *fila);

#endif