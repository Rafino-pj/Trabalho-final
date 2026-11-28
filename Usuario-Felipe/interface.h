/* interface.h - funcoes de tela e de leitura usadas por todo o projeto */
#ifndef INTERFACE_H
#define INTERFACE_H

#include <stddef.h>

#define LARGURA_TELA 50
#define NOME_EMPRESA "nome da empresa"       /* trocar pelo nome do grupo */
#define NOME_PROGRAMA "Sistema de Logistica de Cargas"

/* Recebe: o caractere a repetir. Retorna: nada.
 * Faz: imprime uma linha com LARGURA_TELA repeticoes do caractere. */
void imprimirLinha(char caractere);

/* Recebe: nada. Retorna: nada. Faz: imprime um separador simples. */
void imprimirSeparador(void);

/* Recebe: um texto. Retorna: nada. Faz: imprime o texto centralizado. */
void imprimirCentralizado(const char *texto);

/* Recebe: o titulo da secao. Retorna: nada. Faz: imprime um cabecalho. */
void imprimirCabecalho(const char *titulo);

/* Recebe: nada. Retorna: nada. Faz: imprime a tela de abertura. */
void imprimirTelaAbertura(void);

/* Recebe: mensagem, vetor de destino e tamanho do vetor (minimo 2).
 * Retorna: 1 se leu uma linha, 0 se a entrada terminou ou deu erro.
 * Faz: le com fgets e remove o '\n'. Se o texto nao couber, avisa e
 * pergunta de novo (nunca corta a entrada sem avisar). */
int lerString(const char *mensagem, char *destino, size_t tamanho);

/* Recebe: mensagem e ponteiro para o resultado.
 * Retorna: 1 se leu um inteiro, 0 se a entrada terminou ou deu erro.
 * Faz: repete a pergunta ate o usuario digitar um inteiro valido. */
int lerInteiro(const char *mensagem, int *valor);

/* Recebe: mensagem e ponteiro para o resultado.
 * Retorna: 1 se leu um valor, 0 se a entrada terminou ou deu erro.
 * Faz: aceita virgula ou ponto e repete ate ler um numero nao negativo. */
int lerReal(const char *mensagem, double *valor);

/* Recebe: nada. Retorna: nada. Faz: espera o usuario apertar ENTER. */
void pausar(void);

#endif
