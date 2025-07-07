#ifndef USUARIO_H
#define USUARIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USUARIOS 100
#define MAX_AMIGOS 99
#define MAX_HEAP_SIZE 100
#define FILENAME "usuarios.txt"


typedef struct {
    char username[50];
    char senha_hash[65];
    char musica[50];
    char hobbie[50];
    char amigos[MAX_AMIGOS][50];
    int num_amigos;
} Usuario;

typedef struct {
    Usuario dados[MAX_HEAP_SIZE];
    int tamanho;
} MaxHeap;

typedef struct NoBST {
    Usuario u;
    struct NoBST *esq, *dir;
} NoBST;

typedef struct NoAVL {
    Usuario u;
    struct NoAVL *esq;
    struct NoAVL *dir;
    int altura;
} NoAVL;


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
void listar_amigos(char *username);
void modo_chat(char *user, char *amigo);
void* receber_mensagens(void *arg);
void chat_grupo(char *username);
void sair_grupo(char *username);
void entrar_grupo(char *username);
void criar_grupo(char *dono);
void buscar_usuario_por_nome();
void exibir_ranking_usuarios();
void ver_usuarios_em_ordem();

#endif
