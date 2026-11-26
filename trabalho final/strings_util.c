/*
 * strings_util.c
 * Implementacao das funcoes utilitarias de string.
 * Autor: Gustavo
 */
#include "strings_util.h"

size_t tamanhoString(const char *s) {
    size_t n = 0;
    if (s == NULL) return 0;
    while (s[n] != '\0') n++;
    return n;
}

size_t copiarString(char *destino, const char *origem, size_t tamanhoDestino) {
    size_t i = 0;
    if (destino == NULL || origem == NULL || tamanhoDestino == 0) {
        if (destino != NULL && tamanhoDestino > 0) destino[0] = '\0';
        return 0;
    }
    /* reserva 1 posicao para o '\0' */
    while (origem[i] != '\0' && i < tamanhoDestino - 1) {
        destino[i] = origem[i];
        i++;
    }
    destino[i] = '\0';
    return i;
}

int compararStrings(const char *a, const char *b) {
    size_t i = 0;
    if (a == NULL) a = "";
    if (b == NULL) b = "";
    while (a[i] != '\0' && a[i] == b[i]) i++;
    return (int)((unsigned char)a[i]) - (int)((unsigned char)b[i]);
}

void removerQuebraDeLinha(char *s) {
    size_t n = tamanhoString(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}