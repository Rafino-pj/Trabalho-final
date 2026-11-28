#define _POSIX_C_SOURCE 200809L
#define _POSIX_C_SOURCE 200809L
#include "usuario.h"
#include <stdlib.h>
#include <string.h>

/*
 * Funcao hash interna (metodo da divisao com polinomio simples).
 * Retorna um indice valido entre 0 e TAM_TABELA - 1.
 */
static unsigned int calcularHash(const char* login) {
    unsigned int soma = 0;
    for (int i = 0; login[i] != '\0'; i++) {
        soma = soma * 31 + (unsigned char)login[i];
    }
    return soma % TAM_TABELA;
}

/*
 * Cadastra um usuario na tabela hash.
 * Retornos:
 *  1 -> Cadastro realizado com sucesso
 *  0 -> Erro: ponteiro invalido, tamanho excedido (>= 30 chars) ou login ja cadastrado
 * -1 -> Erro: falha de alocacao de memoria
 */
int cadastrarUsuario(Usuario* novoUsuario, NoUsuario* tabelaUsuarios[TAM_TABELA]) {
    if (novoUsuario == NULL || tabelaUsuarios == NULL) {
        return 0;
    }

    // 1. Blindagem contra buffer overflow:
    // Verifica se login e senha tem no maximo 29 caracteres uteis (+1 para o '\0').
    // strnlen evita ler alem do limite caso a string de entrada nao tenha '\0'.
    size_t tamLogin = strnlen(novoUsuario->login, TAM_LOGIN);
    size_t tamSenha = strnlen(novoUsuario->senha, TAM_SENHA);

    if (tamLogin == 0 || tamLogin >= TAM_LOGIN || tamSenha == 0 || tamSenha >= TAM_SENHA) {
        return 0; // Recusa entrada vazia ou que estoure o limite de 29 caracteres
    }

    unsigned int indice = calcularHash(novoUsuario->login);

    // 2. Verifica se o login ja existe na lista encadeada daquela posicao
    NoUsuario* atual = tabelaUsuarios[indice];
    while (atual != NULL) {
        if (strcmp(atual->info->login, novoUsuario->login) == 0) {
            return 0; // Usuario ja existe
        }
        atual = atual->prox;
    }

    // 3. Aloca memoria para a copia dos dados do usuario
    Usuario* usuarioAlocado = (Usuario*)malloc(sizeof(Usuario));
    if (usuarioAlocado == NULL) {
        return -1; // Falha de memoria
    }

    // Copia garantindo o terminador nulo '\0' no final
    memcpy(usuarioAlocado->login, novoUsuario->login, tamLogin + 1);
    memcpy(usuarioAlocado->senha, novoUsuario->senha, tamSenha + 1);

    // 4. Aloca memoria para o novo no da lista de colisoes
    NoUsuario* novoNo = (NoUsuario*)malloc(sizeof(NoUsuario));
    if (novoNo == NULL) {
        free(usuarioAlocado); // Evita vazamento de memoria
        return -1;
    }

    // 5. Insere o novo no no inicio da lista encadeada (O(1))
    novoNo->info = usuarioAlocado;
    novoNo->prox = tabelaUsuarios[indice];
    tabelaUsuarios[indice] = novoNo;

    return 1; // Sucesso
}

/*
 * Valida login e senha na tabela hash.
 * Retornos:
 *  1 -> Login e senha validos (usuario autenticado)
 *  0 -> Usuario nao encontrado, senha incorreta ou entrada invalida
 */
int validarUsuario(Usuario* dadoslogin, NoUsuario* tabelaUsuarios[TAM_TABELA]) {
    if (dadoslogin == NULL || tabelaUsuarios == NULL) {
        return 0;
    }

    // Protecao tambem na validacao para garantir que a leitura nao passe do limite
    if (strnlen(dadoslogin->login, TAM_LOGIN) >= TAM_LOGIN ||
        strnlen(dadoslogin->senha, TAM_SENHA) >= TAM_SENHA) {
        return 0;
    }

    unsigned int indice = calcularHash(dadoslogin->login);

    // Percorre apenas a lista encadeada correspondente ao hash do login
    NoUsuario* atual = tabelaUsuarios[indice];
    while (atual != NULL) {
        if (strcmp(atual->info->login, dadoslogin->login) == 0) {
            if (strcmp(atual->info->senha, dadoslogin->senha) == 0) {
                return 1; // Autenticado
            }
            return 0; // Senha incorreta
        }
        atual = atual->prox;
    }

    return 0; // Login nao encontrado
}
