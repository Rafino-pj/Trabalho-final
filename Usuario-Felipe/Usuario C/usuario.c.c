#include "usuario.h"
#include <stdlib.h>
#include <string.h>

/*
 * Funcao hash interna (metodo da divisao com soma simples de caracteres).
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
 *  0 -> Erro: login ja cadastrado ou ponteiro invalido
 * -1 -> Erro: falha de alocacao de memoria
 */
int cadastrarUsuario(Usuario* novoUsuario, NoUsuario* tabelaUsuarios[TAM_TABELA]) {
    if (novoUsuario == NULL || tabelaUsuarios == NULL) {
        return 0;
    }

    unsigned int indice = calcularHash(novoUsuario->login);

    // 1. Verifica se o login ja existe na lista encadeada daquela posicao
    NoUsuario* atual = tabelaUsuarios[indice];
    while (atual != NULL) {
        if (strcmp(atual->info->login, novoUsuario->login) == 0) {
            return 0; // Usuario ja existe
        }
        atual = atual->prox;
    }

    // 2. Aloca memoria para a copia dos dados do usuario
    Usuario* usuarioAlocado = (Usuario*)malloc(sizeof(Usuario));
    if (usuarioAlocado == NULL) {
        return -1; // Falha de memoria
    }
    strcpy(usuarioAlocado->login, novoUsuario->login);
    strcpy(usuarioAlocado->senha, novoUsuario->senha);

    // 3. Aloca memoria para o novo no da lista de colisoes
    NoUsuario* novoNo = (NoUsuario*)malloc(sizeof(NoUsuario));
    if (novoNo == NULL) {
        free(usuarioAlocado); // Evita vazamento de memoria
        return -1;
    }

    // 4. Insere o novo no no inicio da lista encadeada (O(1))
    novoNo->info = usuarioAlocado;
    novoNo->prox = tabelaUsuarios[indice];
    tabelaUsuarios[indice] = novoNo;

    return 1; // Sucesso
}

/*
 * Valida login e senha na tabela hash.
 * Retornos:
 *  1 -> Login e senha validos (usuario autenticado)
 *  0 -> Usuario nao encontrado ou senha incorreta
 */
int validarUsuario(Usuario* dadoslogin, NoUsuario* tabelaUsuarios[TAM_TABELA]) {
    if (dadoslogin == NULL || tabelaUsuarios == NULL) {
        return 0;
    }

    unsigned int indice = calcularHash(dadoslogin->login);

    // Percorre apenas a lista encadeada correspondente ao hash do login
    NoUsuario* atual = tabelaUsuarios[indice];
    while (atual != NULL) {
        if (strcmp(atual->info->login, dadoslogin->login) == 0) {
            // Login encontrado; verifica se a senha confere
            if (strcmp(atual->info->senha, dadoslogin->senha) == 0) {
                return 1; // Autenticado
            }
            return 0; // Senha incorreta
        }
        atual = atual->prox;
    }

    return 0; // Login nao encontrado
}
