#ifndef USUARIO_H
#define USUARIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <process.h>
#include <locale.h>
#include <conio.h>

#define FILENAME "usuarios.txt"
#define MAX_CARACTERISTICAS 10
#define MAX_CARACTERISTICA 30
#define MAX_USUARIOS 100
#define MAX_NOME 20
#define MAX_SENHA 20


typedef struct {
    char username[MAX_NOME];
    char senha[MAX_SENHA];
    char caracteristicas[MAX_CARACTERISTICAS][MAX_CARACTERISTICA];
    int numCaracteristicas;
    char amigos[MAX_USUARIOS][MAX_NOME];
    int numAmigos;
    char pedidos[MAX_USUARIOS][MAX_NOME];
    int numPedidos;
    int seguidores;
    int seguindo;
    char seguindoUsuarios[MAX_USUARIOS][MAX_NOME];
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
void sugerir_amigos(Usuario usuarios[], int total, Usuario *usuario);
void editar_perfil(Usuario usuarios[], Usuario *usuario, int totalUsuarios);
void trocar_senha(Usuario usuarios[], int total, Usuario *usuario);
void remover_amigo(Usuario usuarios[], int total, Usuario *usuario);
void seguir_usuario(Usuario usuarios[], int total, Usuario *usuario);
void grau_popularidade(Usuario *usuario);
void mostrar_influencers(Usuario usuarios[], int total);
void apagar_conta(Usuario usuarios[], int total, Usuario *usuario);
void criar_chat(const char *u1, const char *u2, char *nomeArquivo);
void carregar_historico_mensagens(const char *arquivo);
void __cdecl carregar_mensagens(void *args);
void chat(Usuario usuarios[], Usuario *remetente, int total);
void carregar_mensagens(void *arg);
int distancia_minima(Usuario usuarios[], int total, char *u1, char *u2);
void esconder_senha(char *password);
void atualizar_dados_usuarios(Usuario usuarios[], int *total);
#endif
