#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

void inicializarFila(Fila *fila)
{
    if (fila == NULL)
        return;

    fila->inicio = NULL;
    fila->fim = NULL;
}

int validarCPF(const char *cpf)
{

    if (cpf == NULL)
        return 0;

    if (strlen(cpf) != 11)
        return 0;

    for (int i = 0; i < 11; i++)
    {
        if (cpf[i] < '0' || cpf[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}

int filaVazia(const Fila *fila)
{

    if (fila == NULL)
        return 1;

    return fila->inicio == NULL;
}

int enfileirarEntrega(Fila *fila, Entrega entrega)
{

    NoEntrega *novo;

    if (fila == NULL)
        return 0;

    if (entrega.idProduto <= 0)
        return 0;

    if (entrega.nomeDestinatario[0] == '\0')
        return 0;

    if (!validarCPF(entrega.cpf))
    {
        printf("CPF invalido! Digite 11 numeros.\n");
        return 0;
    }

    novo = (NoEntrega *)malloc(sizeof(NoEntrega));

    if (novo == NULL)
        return -1;

    novo->dados = entrega;
    novo->proximo = NULL;

    if (fila->inicio == NULL)
    {

        fila->inicio = novo;
        fila->fim = novo;
    }
    else
    {

        fila->fim->proximo = novo;
        fila->fim = novo;
    }

    return 1;
}

Entrega desenfileirarEntrega(Fila *fila)
{

    Entrega entrega = {0, "", ""};

    if (filaVazia(fila))
    {
        printf("Nenhuma entrega disponivel na fila.\n");
        return entrega;
    }

    NoEntrega *aux = fila->inicio;

    entrega = aux->dados;

    fila->inicio = aux->proximo;

    if (fila->inicio == NULL)
    {
        fila->fim = NULL;
    }

    free(aux);

    return entrega;
}

void listarFila(const Fila *fila)
{

    NoEntrega *atual;

    if (fila == NULL || fila->inicio == NULL)
    {
        printf("Nenhuma entrega cadastrada.\n");
        return;
    }

    printf("%-10s | %-11s | %-30s\n",
           "ID PRODUTO", "CPF", "DESTINATARIO");

    printf("----------------------------------------------------------\n");

    for (atual = fila->inicio; atual != NULL; atual = atual->proximo)
    {

        printf("%-10d | %-11s | %-30s\n",
               atual->dados.idProduto,
               atual->dados.cpf,
               atual->dados.nomeDestinatario);
    }
}

void liberarFila(Fila *fila)
{

    NoEntrega *atual;
    NoEntrega *proximo;

    if (fila == NULL)
        return;

    atual = fila->inicio;

    while (atual != NULL)
    {

        proximo = atual->proximo;

        free(atual);

        atual = proximo;
    }

    fila->inicio = NULL;
    fila->fim = NULL;
}