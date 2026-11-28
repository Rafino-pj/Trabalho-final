/* interface.c - implementacao das funcoes de tela e de leitura */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

void imprimirLinha(char caractere)
{
    int i;

    for (i = 0; i < LARGURA_TELA; i++) {
        putchar(caractere);
    }
    putchar('\n');
}

void imprimirSeparador(void)
{
    imprimirLinha('-');
}

void imprimirCentralizado(const char *texto)
{
    int espacos = (LARGURA_TELA - (int)strlen(texto)) / 2;

    if (espacos < 0) {
        espacos = 0;
    }
    /* "%*s" com texto vazio imprime a quantidade de espacos pedida */
    printf("%*s%s\n", espacos, "", texto);
}

void imprimirCabecalho(const char *titulo)
{
    putchar('\n');
    imprimirLinha('=');
    imprimirCentralizado(titulo);
    imprimirLinha('=');
}

void imprimirTelaAbertura(void)
{
    imprimirLinha('*');
    imprimirLinha('=');
    imprimirCentralizado(NOME_EMPRESA);
    imprimirCentralizado(NOME_PROGRAMA);
    imprimirLinha('=');
    imprimirLinha('*');
}

int lerString(const char *mensagem, char *destino, size_t tamanho)
{
    size_t fim;
    int c;

    if (destino == NULL || tamanho < 2) {
        return 0;
    }

    for (;;) {
        printf("%s", mensagem);
        fflush(stdout);

        if (fgets(destino, (int)tamanho, stdin) == NULL) {
            destino[0] = '\0';
            return 0;
        }

        fim = strcspn(destino, "\n");
        if (destino[fim] == '\n') {
            destino[fim] = '\0';
            return 1;
        }

        /* o '\n' nao coube: verifica se sobrou texto na linha */
        c = getchar();
        if (c == '\n' || c == EOF) {
            return 1;
        }
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        printf("Texto muito longo. Use no maximo %d caracteres.\n",
               (int)tamanho - 1);
    }
}

int lerInteiro(const char *mensagem, int *valor)
{
    char linha[32];
    char *resto;
    long numero;

    while (lerString(mensagem, linha, sizeof linha)) {
        errno = 0;
        numero = strtol(linha, &resto, 10);

        if (resto != linha && *resto == '\0' && errno == 0
                && numero >= INT_MIN && numero <= INT_MAX) {
            *valor = (int)numero;
            return 1;
        }
        printf("Entrada invalida. Digite um numero inteiro.\n");
    }
    return 0;
}

int lerReal(const char *mensagem, double *valor)
{
    char linha[32];
    char *virgula;
    char *resto;
    double numero;

    while (lerString(mensagem, linha, sizeof linha)) {
        virgula = strchr(linha, ',');
        if (virgula != NULL) {
            *virgula = '.';
        }
        numero = strtod(linha, &resto);

        /* o limite tambem rejeita "inf" e "nan" */
        if (resto != linha && *resto == '\0'
                && numero >= 0 && numero <= 1e9) {
            *valor = numero;
            return 1;
        }
        printf("Valor invalido. Exemplo: 49,90\n");
    }
    return 0;
}

void pausar(void)
{
    int c;

    printf("\nPressione ENTER para continuar...");
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
