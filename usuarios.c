#include "usuarios.h"

#define MAX_USUARIOS 100
#define FILENAME "usuarios.txt"

void hash_senha(char *senha, char *resultado) {
    int i;
    for(i=0; senha[i] != '\0'; i++) {
        resultado[i] = senha[i] + 3;
    }
    resultado[i] = '\0';
}

void salvar_usuario(Usuario u) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("[X] Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(file, "%s %s\n", u.username, u.senha_hash);
    fclose(file);
}

int carregar_usuarios(Usuario usuarios[], int max) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return 0;

    int count = 0;
    while (fscanf(file, "%s %s", usuarios[count].username, usuarios[count].senha_hash) == 2) {
        count++;
        if (count >= max) break;
    }
    fclose(file);
    return count;
}

int usuario_existe(char *username) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, username) == 0) {
            return 1;
        }
    }
    return 0;
}

int login(char *username, char *senha) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    int i;
    char senha_hash[65];
    hash_senha(senha, senha_hash);

    for (i=0; i<total; i++) {
        if (strcmp(usuarios[i].username, username) == 0 &&
            strcmp(usuarios[i].senha_hash, senha_hash) == 0) {
            return 1;
        }
    }
    return 0;
}

void cadastrar() {
    Usuario novo;
    char senha[50];

    printf("------------------------------------------\n");
    printf("                Criar conta                 \n");
    printf("------------------------------------------\n\n");
    printf("[?] Nome de usuário: ");
    scanf("%s", novo.username);

    if (usuario_existe(novo.username)) {
        printf("[X] Usuário já existe!\n");
        return;
    }

    printf("[?] Senha: ");
    scanf("%s", senha);

    hash_senha(senha, novo.senha_hash);
    salvar_usuario(novo);

    printf("[!] Usuário cadastrado com sucesso!\n");
}

void alterar_senha(char *username) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    char nova_senha[50];
    char nova_hash[65];

    printf("------------------------------------------\n");
    printf("              Alterar senha              \n");
    printf("------------------------------------------\n\n");
    printf("[!] Nova senha: ");
    scanf("%s", nova_senha);
    hash_senha(nova_senha, nova_hash);

    int encontrado = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, username) == 0) {
            strcpy(usuarios[i].senha_hash, nova_hash);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("[X] Usuário não encontrado.\n");
        return;
    }

    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("[X] Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    int i;

    for (i=0; i < total; i++) {
        fprintf(file, "%s %s\n", usuarios[i].username, usuarios[i].senha_hash);
    }

    fclose(file);
    printf("[!] Senha alterada com sucesso!\n");
}

void logado(char *username) {
    char op2;

    do 
    {
        printf("Olá, @%s \n", username);
        printf("[1] Alterar senha\n");
        printf("[0] Logout\n");
        scanf(" %c", &op2);

        switch(op2) 
        {
            case '1':
                alterar_senha(username);
                break;
            case '0':
                printf("Fazendo logout...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(op2 != '0');
}

void fazer_login() {
    char username[50], senha[50];

    printf("------------------------------------------\n");
    printf("   Insira as suas informações de Login   \n");
    printf("------------------------------------------\n\n");

    
    printf("[?] Usuário: ");
    scanf("%s", username);
    printf("[?] Senha: ");
    scanf("%s", senha);
    
    system("cls");
    
    if (login(username, senha)) {
        printf("[!] Login realizado com sucesso! Bem-vindo, %s.\n", username);
        system("pause");
        system("cls");
        logado(username);
    } else {
        printf("[X] Usuário ou senha incorretos.\n");
    }
}

