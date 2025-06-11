#ifndef USUARIO_H
#define USUARIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char username[50];
    char senha_hash[65];
} Usuario;

void hash_senha(char *senha, char *resultado);
int login(char *username, char *senha);
int usuario_existe(char *username);
void salvar_usuario(Usuario u);
int carregar_usuarios(Usuario usuarios[], int max);
void cadastrar();
void fazer_login();
void alterar_senha(char *username);
void logado(char *username);

#endif
