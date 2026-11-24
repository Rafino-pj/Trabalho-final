#ifndef USUARIO_H
#define USUARIO_H

#define TAM_TABELA 10
#define TAM_LOGIN 30
#define TAM_SENHA 30

/* Dados de um usuario cadastrado */
typedef struct Usuario {
    char login[TAM_LOGIN];
    char senha[TAM_SENHA];
} Usuario;

/* No da lista de colisoes de cada posicao da tabela */
typedef struct NoUsuario {
    struct Usuario* info;
    struct NoUsuario* prox;
} NoUsuario;

/* Cadastra um usuario na tabela hash */
int cadastrarUsuario(Usuario* novoUsuario, NoUsuario* tabelaUsuarios[TAM_TABELA]);

/* Valida login e senha */
int validarUsuario(Usuario* dadoslogin, NoUsuario* tabelaUsuarios[TAM_TABELA]);

#endif
```
