/*
- arvore.c
- Implementacao da arvore binaria de busca de cargas.
- Responsavel: Vinicius.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

static int dataValida(const char *data)
{
    int i, dia, mes;

    if (data == NULL || strlen(data) != 10)
        return 0;

    for (i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
        {
            if (data[i] != '/')
                return 0;
        }
        else if (data[i] < '0' || data[i] > '9')
        {
            return 0;
        }
    }

    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');

    return dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12;
}

static void imprimirCarga(const Carga *carga)
{
    int i;

    printf("Carga %d | Saida: %s | Entregas: %d/%d\n",
           carga->idCarga,
           carga->diaSaidaCaminhao,
           carga->qtdEntregas,
           MAX_ENTREGAS_CARGA);

    if (carga->qtdEntregas <= 0)
    {
        printf("  Nenhuma entrega na carga.\n");
        return;
    }

    for (i = 0; i < carga->qtdEntregas; i++)
    {
        printf("  %d) Produto %d | CPF %s | %s\n",
               i + 1,
               carga->entregas[i].idProduto,
               carga->entregas[i].cpfDestinatario,
               carga->entregas[i].nomeDestinatario);
    }
}

int inserirCarga(Carga **raiz, Carga novaCarga)
{
    Carga *atual, *novo;

    if (raiz == NULL)
        return 0;

    if (novaCarga.idCarga <= 0 || !dataValida(novaCarga.diaSaidaCaminhao))
        return 0;

    if (novaCarga.qtdEntregas < 0 || novaCarga.qtdEntregas > MAX_ENTREGAS_CARGA)
        return 0;

    if (buscarCarga(*raiz, novaCarga.idCarga) != NULL)
        return 0;

    novo = (Carga *)malloc(sizeof(Carga));
    if (novo == NULL)
        return -1;

    *novo = novaCarga;
    novo->esquerda = NULL;
    novo->direita = NULL;

    if (*raiz == NULL)
    {
        *raiz = novo;
        return 1;
    }

    atual = *raiz;
    while (atual != NULL)
    {
        if (novaCarga.idCarga < atual->idCarga)
        {
            if (atual->esquerda == NULL)
            {
                atual->esquerda = novo;
                return 1;
            }
            atual = atual->esquerda;
        }
        else if (novaCarga.idCarga > atual->idCarga)
        {
            if (atual->direita == NULL)
            {
                atual->direita = novo;
                return 1;
            }
            atual = atual->direita;
        }
        else
        {
            free(novo);
            return 0;
        }
    }

    free(novo);
    return 0;
}

Carga *buscarCarga(Carga *raiz, int idCarga)
{
    Carga *atual = raiz;

    while (atual != NULL)
    {
        if (idCarga == atual->idCarga)
            return atual;
        else if (idCarga < atual->idCarga)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    return NULL;
}

void listarCargas(const Carga *raiz)
{
    if (raiz == NULL)
        return;

    listarCargas(raiz->esquerda);
    imprimirCarga(raiz);
    printf("----------------------------------------\n");
    listarCargas(raiz->direita);
}

int buscarCargasPorData(const Carga *raiz, const char *diaSaidaCaminhao)
{
    int achadas = 0;

    if (raiz == NULL || diaSaidaCaminhao == NULL)
        return 0;

    achadas += buscarCargasPorData(raiz->esquerda, diaSaidaCaminhao);

    if (strcmp(raiz->diaSaidaCaminhao, diaSaidaCaminhao) == 0)
    {
        imprimirCarga(raiz);
        achadas++;
    }

    achadas += buscarCargasPorData(raiz->direita, diaSaidaCaminhao);

    return achadas;
}

void liberarArvore(Carga **raiz)
{
    if (raiz == NULL || *raiz == NULL)
        return;

    liberarArvore(&((*raiz)->esquerda));
    liberarArvore(&((*raiz)->direita));

    free(*raiz);
    *raiz = NULL;
}
