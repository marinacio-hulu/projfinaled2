#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "usuarios.h"


char grupo_chat_file[100];
volatile int continuar_grupo = 1;
long pos_grupo = 0;

void criar_grupo(char *dono) {
    char nome[50];
    printf("Nome do grupo: ");
    scanf("%s", nome);

    char members_file[100];
    sprintf(members_file, "grupo_%s_members.txt", nome);

    FILE *file = fopen(members_file, "w");
    if (!file) {
        printf("[X] Erro ao criar grupo.\n");
        return;
    }

    fprintf(file, "%s\n", dono);  // dono é o primeiro membro
    fclose(file);
    printf("[!] Grupo '%s' criado com sucesso!\n", nome);
}

void entrar_grupo(char *username) {
    char nome[50];
    printf("Nome do grupo para entrar: ");
    scanf("%s", nome);

    char members_file[100];
    sprintf(members_file, "grupo_%s_members.txt", nome);

    FILE *file = fopen(members_file, "a+");
    if (!file) {
        printf("[X] Grupo não encontrado.\n");
        return;
    }

    char membro[50];
    while (fscanf(file, "%s", membro) == 1) {
        if (strcmp(membro, username) == 0) {
            printf("[!] Você já está no grupo.\n");
            fclose(file);
            return;
        }
    }

    fprintf(file, "%s\n", username);
    fclose(file);
    printf("[+] Você entrou no grupo '%s'.\n", nome);
}

void sair_grupo(char *username) {
    char nome[50];
    printf("Nome do grupo para sair: ");
    scanf("%s", nome);

    char members_file[100];
    sprintf(members_file, "grupo_%s_members.txt", nome);

    FILE *in = fopen(members_file, "r");
    if (!in) {
        printf("[X] Grupo não encontrado.\n");
        return;
    }

    FILE *out = fopen("temp.txt", "w");
    char membro[50];
    int found = 0;

    while (fscanf(in, "%s", membro) == 1) {
        if (strcmp(membro, username) != 0)
            fprintf(out, "%s\n", membro);
        else
            found = 1;
    }

    fclose(in);
    fclose(out);

    remove(members_file);
    rename("temp.txt", members_file);

    if (found)
        printf("[-] Você saiu do grupo '%s'.\n", nome);
    else
        printf("[!] Você não estava no grupo.\n");
}

void* receber_grupo(void *arg) {
    FILE *file;
    char linha[300];

    while (continuar_grupo) {
        file = fopen(grupo_chat_file, "r");
        if (file) {
            fseek(file, pos_grupo, SEEK_SET);
            while (fgets(linha, sizeof(linha), file)) {
                printf("\r%s", linha);
                printf("> "); fflush(stdout);
            }
            pos_grupo = ftell(file);
            fclose(file);
        }
        sleep(1);
    }
    return NULL;
}
void chat_grupo(char *username) {
    char nome[50];
    printf("Nome do grupo: ");
    scanf("%s", nome);
    getchar(); // limpa o '\n'

    // arquivos do grupo
    char members_file[100];
    sprintf(members_file, "grupo_%s_members.txt", nome);
    sprintf(grupo_chat_file, "grupo_%s_chat.txt", nome);

    // Verificar se é membro
    FILE *mfile = fopen(members_file, "r");
    if (!mfile) {
        printf("[X] Grupo não encontrado.\n");
        return;
    }

    int membro = 0;
    char buffer[50];
    while (fscanf(mfile, "%s", buffer) == 1) {
        if (strcmp(buffer, username) == 0) {
            membro = 1;
            break;
        }
    }
    fclose(mfile);

    if (!membro) {
        printf("[X] Você não faz parte desse grupo.\n");
        return;
    }

    printf("Entrando no grupo '%s'. Digite '/sair' para sair.\n", nome);

    // inicializa thread de recebimento
    pthread_t tid;
    continuar_grupo = 1;
    pos_grupo = 0;
    pthread_create(&tid, NULL, receber_grupo, NULL);

    // envio de mensagens
    char msg[256];
    while (1) {
        printf("> ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;

        if (strcmp(msg, "/sair") == 0) {
            continuar_grupo = 0;
            break;
        }

        FILE *file = fopen(grupo_chat_file, "a");
        if (file) {
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            char data_hora[20];
            strftime(data_hora, sizeof(data_hora), "%d/%m/%Y %H:%M", tm_info);
            fprintf(file, "[%s] @%s: %s\n", data_hora, username, msg);
            fclose(file);
        }
    }

    pthread_join(tid, NULL);
    printf("Você saiu do grupo '%s'.\n", nome);
}