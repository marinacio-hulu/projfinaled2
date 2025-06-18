#include "usuario.h"
/* -----------------------------------------------
 Função para criptografar as senhas usando
 cifra de césar, a função aumenta +3 posições
 a cada caracter e retorna a string criptografada
 ----------------------------------------------- */
void hash_senha(char *senha, char *resultado) {
    int i;
    for (i = 0; senha[i] != '\0'; i++) {
        resultado[i] = senha[i] + 3;
    }
    resultado[i] = '\0';
}

/* -----------------------------------------------
 Função para carregar os usuários do arquivo
 ----------------------------------------------- */
int carregar_usuarios(Usuario usuarios[], int *total) {
    FILE *f = fopen(FILENAME, "rb");
    if (!f) return 0;

    fread(total, sizeof(int), 1, f);
    fread(usuarios, sizeof(Usuario), *total, f);
    printf("%s", usuarios[0].username);
    exit(0);
    fclose(f);
    return 1;
}

/* -----------------------------------------------
 Função para salvar os usuários
 ----------------------------------------------- */
int salvar_usuarios(Usuario usuarios[], int total) {
    FILE *f = fopen(FILENAME, "wb");
    if (!f) return 0;

    fwrite(&total, sizeof(int), 1, f);
    fwrite(usuarios, sizeof(Usuario), total, f);
    fclose(f);
    return 1;
}


/* -----------------------------------------------
 Função para buscar um usuário
 ----------------------------------------------- */
int encontrar_usuario(Usuario usuarios[], int total, const char *username) {
    int i;
    for (i = 0; i < total; i++) {
        if (usuarios[i].ativo && strcmp(usuarios[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

/* -----------------------------------------------
 Função para cadastrar um usuário na rede
 ----------------------------------------------- */
void cadastrar_usuario(Usuario usuarios[], int *totalUsuarios) {
    Usuario novo;
    char senha[50];

    printf("[?] Nome de usuário: ");
    scanf(" %s", novo.username);

    if (encontrar_usuario(usuarios, *totalUsuarios, novo.username) != -1) {
        printf("[!] Usuário já existe.\n");
        return;
    }

    printf("[?] Senha: ");
    scanf(" %s", senha);
    hash_senha(senha, novo.senha);

    printf("[?] Quantas características? ");
    scanf("%d", &novo.numCaracteristicas);

    for (int i = 0; i < novo.numCaracteristicas; i++) {
        printf("[?] Característica %d: ", i + 1);
        scanf(" %s", novo.caracteristicas[i]);
    }

    novo.numAmigos = 0;
    novo.seguidores = 0;
    novo.seguindo = 0;
    novo.ativo = 1;

    usuarios[*totalUsuarios] = novo;
    (*totalUsuarios)++;

    salvar_usuarios(usuarios, *totalUsuarios);
    printf("[!] Usuário cadastrado com sucesso!\n");
}

/* -----------------------------------------------
 Função fazer o login na rede
 ----------------------------------------------- */
int login(Usuario usuarios[], int totalUsuarios) {
    char user[50], senha[20], senha_hash[20];
    printf("[?] Username: ");
    scanf(" %s", user);
    printf("[?] Senha: ");
    scanf(" %s", senha);
    system("cls");

    hash_senha(senha, senha_hash);

    int idx = encontrar_usuario(usuarios, totalUsuarios, user);
    if (idx != -1 && strcmp(usuarios[idx].senha, senha_hash) == 0) {
        printf("[!] Login bem-sucedido!\n");
        system("pause");
        system("cls");

        srs(usuarios, &usuarios[idx], totalUsuarios);
        return idx;
    }

    printf("[x] Username ou Senha incorrectos.\n");
    return -1;
}

/* -----------------------------------------------
 menu
 ----------------------------------------------- */
void menu(Usuario usuarios[], int totalUsuarios) {
    char op;
    do {
        printf("\n\t--- SRS ---\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Login\n");
        printf("0. Sair\n");
        printf("-> ");
        scanf(" %c", &op);
        system("cls");

        if (op=='1') {
            cadastrar_usuario(usuarios, &totalUsuarios);
        } else if(op=='2') {
            login(usuarios, totalUsuarios);
        } else if(op=='0') {
            exit(1);
        } else {
            printf("[x] Opção inválida\n");
        }
        system("pause");
        system("cls");
    } while (op != '0');
}

/* -----------------------------------------------
 Função para mostrar o perfil de um usuário
 ----------------------------------------------- */
void mostrar_perfil(Usuario *usuario) {
    printf("------------------------------------------\n");
    printf("           @%-20s          \n", usuario->username);
    printf("------------------------------------------\n\n");

    printf("+------------------------------------------+\n");
    printf("| Username: @%-20s          |\n", usuario->username);
    printf("+------------------------------------------+\n");
    printf("| Caracteristicas: %-3d                     |\n", usuario->numCaracteristicas);
    printf("+------------------------------------------+\n");
    printf("| Amigos: %-3d                              |\n", usuario->numAmigos);
    printf("+------------------------------------------+\n");
    printf("| Seguidores: %-3d                          |\n", usuario->seguidores);
    printf("+------------------------------------------+\n");
    printf("| Seguindo: %-3d                            |\n", usuario->seguidores);
    printf("+------------------------------------------+\n\n");
}

void srs(Usuario usuarios[], Usuario *usuario, int totalUsuarios) {
    char op2;
    do {
        printf("------------------------------------------\n");
        printf("                @%s                 \n", usuario->username);
        printf("------------------------------------------\n\n");
        printf("1. Ver perfil\n");
        printf("2. Procurar perfil\n");
        printf("3. Editar perfil\n");
        printf("0. Terminar sessão\n");
        printf("-> ");
        scanf(" %c", &op2);
        system("cls");

        if (op2=='1') {
            mostrar_perfil(usuario);
        } else if(op2=='2') {
            char perfil[20];
            printf("[?] Insira o username: ");
            scanf("%s", perfil);
            system("cls");

            int idx = encontrar_usuario(usuarios, totalUsuarios, perfil);
            if (idx != -1 && strcmp(usuarios[idx].username, perfil) == 0) {
                // tá dar erro aqui
                
                mostrar_perfil(&usuarios[idx]);
            }


        } else if(op2=='0') {
            printf("[!] Terminado a sessão...\n");
        } else {
            printf("[x] Opção inválida\n");
        }
        
        system("pause");
        system("cls");
    } while (op2 != '0');
}
