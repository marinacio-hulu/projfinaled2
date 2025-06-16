#ifndef USUARIO_H
#define USUARIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USUARIOS 100
#define MAX_AMIGOS 99
#define FILENAME "usuarios.txt"


typedef struct {
    char username[50];
    char senha_hash[65];
    char musica[50];
    char hobbie[50];
    char amigos[MAX_AMIGOS][50];
    int num_amigos;
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
void ver_perfil(char *username);

#endif
