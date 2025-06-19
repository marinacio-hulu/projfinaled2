#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FILENAME "usuarios.txt"
#define MAX_CARACTERISTICAS 10
#define MAX_USUARIOS 100
#define MAX_NOME 20

typedef struct {
    char username[MAX_NOME];
    char senha[20];
    char caracteristicas[MAX_CARACTERISTICAS][30];
    int numCaracteristicas;
    char amigos[MAX_USUARIOS][MAX_NOME];
    int numAmigos;
    char pedidos[MAX_USUARIOS][MAX_NOME]; // Novos: pedidos de amizade recebidos
    int numPedidos;
    int seguidores;
    int seguindo;
    int ativo;
} Usuario;

int carregar_usuarios(Usuario usuarios[], int *total);
int salvar_usuarios(Usuario usuarios[], int total);
int encontrar_usuario(Usuario usuarios[], int total, const char *username);
int login(Usuario usuarios[], int *totalUsuarios);
void menu(Usuario usuarios[], int *totalUsuarios);
void hash_senha(char *senha, char *resultado);
void conectC(Usuario usuarios[], Usuario *usuario, int totalUsuarios);
void mostrar_perfil(Usuario *usuario);
void enviar_pedido_amizade(Usuario *usuarios, int total, Usuario *remetente);
void ver_pedidos_amizade(Usuario *usuarios, int total, Usuario *usuario);
void mostrar_todos_usuarios(Usuario usuarios[], int *total);
#endif
