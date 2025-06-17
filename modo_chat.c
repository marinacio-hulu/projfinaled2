#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "usuarios.h"

char filename[100];
char user_atual[50];
volatile int continuar_chat = 1;
long last_pos = 0;

void* receber_mensagens(void *arg) {
    FILE *file;
    char linha[300];

    while (continuar_chat) {
        file = fopen(filename, "r");
        if (file) {
            fseek(file, last_pos, SEEK_SET);
            while (fgets(linha, sizeof(linha), file)) {
                printf("\r%s", linha);
                printf("> "); // reaparece o prompt
                fflush(stdout);
            }
            last_pos = ftell(file);
            fclose(file);
        }
        sleep(1); // espera para checar novas mensagens
    }
    return NULL;
}

void modo_chat(char *user, char *amigo) {
    char input[256];
    pthread_t thread_id;

    sprintf(filename, "chat_%s_%s.txt", user, amigo);
    if (strcmp(user, amigo) > 0) {
        sprintf(filename, "chat_%s_%s.txt", amigo, user); // ordem alfabética
    }

    strcpy(user_atual, user);
    continuar_chat = 1;
    last_pos = 0;

    printf("Entrando no chat com @%s. Escreva '/sair' para sair.\n", amigo);

    // Inicia a thread que escuta novas mensagens
    pthread_create(&thread_id, NULL, receber_mensagens, NULL);

    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // remove newline

        if (strcmp(input, "/sair") == 0) {
            continuar_chat = 0;
            break;
        }

        FILE *file = fopen(filename, "a");
        if (file) {
            fprintf(file, "@%s: %s\n", user, input);
            fclose(file);
        }
    }

    pthread_join(thread_id, NULL);
    printf("Chat encerrado.\n");
}
