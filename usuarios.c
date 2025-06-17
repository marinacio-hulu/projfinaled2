#include "usuarios.h"

/* -----------------------------------------------
 Função para criptografar as senhas usando
 cifra de césar, a função aumenta +3 posições
 a cada caracter e retorna a string criptografada
 ----------------------------------------------- */
void hash_senha(char *senha, char *resultado) {
    int i;
    for(i=0; senha[i] != '\0'; i++) {
        resultado[i] = senha[i] + 3;
    }
    resultado[i] = '\0';
}

/* -----------------------------------------------
 Função para salvar os usuários no arquivo com
 todas as informações: usuario, senha, hobby...
 ----------------------------------------------- */
void salvar_usuario(Usuario u) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("[X] Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(file, "%s %s %s %s %d", u.username, u.senha_hash, u.musica, u.hobbie, u.num_amigos);
    
    /* vou testar algumas cenas aqui
    for (int i = 0; i < u.num_amigos; i++) {
        fprintf(file, " %s", u.amigos[i]);
    }*/

    fprintf(file, "\n");
    fclose(file);

    
}

/* -----------------------------------------------
 Função para salvar os usuários do arquivo
 ----------------------------------------------- */
int carregar_usuarios(Usuario usuarios[], int max) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return 0;

    int count = 0;
    while (count < max) {
        Usuario u;
        int res = fscanf(file, "%s %s %s %s %d", u.username, u.senha_hash, u.musica, u.hobbie, &u.num_amigos);
        if (res != 5) break;

        for (int i = 0; i < u.num_amigos; i++) {
            fscanf(file, "%s", u.amigos[i]);
        }

        usuarios[count++] = u;
    }

    fclose(file);
    return count;
}

/* -----------------------------------------------
 Função auxiliar para salvar todos os usuários,
 carrega os usuários, modifica e reescreve o
 arquivo
 ----------------------------------------------- */
void salvar_todos_usuarios(Usuario usuarios[], int total) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("[X] Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        Usuario u = usuarios[i];
        fprintf(file, "%s %s %s %s %d", u.username, u.senha_hash, u.musica, u.hobbie, u.num_amigos);
        for (int j = 0; j < u.num_amigos; j++) {
            fprintf(file, " %s", u.amigos[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/* -----------------------------------------------
 Função para verificar se o usuário existe
 na hora de cadastrar um novo usuário na rede,
 a fim de impedir usuários repetidos
 ----------------------------------------------- */
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

/* -----------------------------------------------
 Função fazer o login na rede
 ----------------------------------------------- */
int login(char *username, char *senha) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    int i;
    char senha_hash[65];
    hash_senha(senha, senha_hash);

    for (i=0; i<total; i++) {
        if (strcmp(usuarios[i].username, username) == 0 &&
            strcmp(usuarios[i].senha_hash, senha_hash) == 0) {
            printf("deu 1\n");
            return 1;
        }
    }
    printf("deu 0\n");
    return 0;
}

/* -----------------------------------------------
 Função para cadastrar um usuário na rede
 ----------------------------------------------- */
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
    
    printf("[?] Gosto musical: ");
    scanf("%s", novo.musica);

    printf("[?] Hobbie: ");
    scanf("%s", novo.hobbie);

    hash_senha(senha, novo.senha_hash);

    novo.num_amigos = 0;

    salvar_usuario(novo);

    printf("[!] Usuário cadastrado com sucesso!\n");
}

/* -----------------------------------------------
 Função alterar a senha
 ----------------------------------------------- */
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
    system("clear");

    hash_senha(nova_senha, nova_hash);

    int encontrado = 0;
    int i;

    for (i=0; i < total; i++) {
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

    for (i=0; i < total; i++) {
        fprintf(file, "%s %s\n", usuarios[i].username, usuarios[i].senha_hash);
    }

    fclose(file);
    printf("[!] Senha alterada com sucesso!\n");
}
/* -----------------------------------------------
 Função para adicionar amigo, não tá 100% pronto
 tou a testar a ainda algumas cenas
 ----------------------------------------------- */
void adicionar_amigo(Usuario usuarios[], int total, char *origem, char *destino) {
    int i, j;
    for (i=0; i<total; i++) {
        if (strcmp(usuarios[i].username, origem) == 0) {
            for (j=0; j<total; j++) {
                if (strcmp(usuarios[j].username, destino) == 0) {
                    int k;
                    for (k=0; k < usuarios[i].num_amigos; k++) {
                        if (strcmp(usuarios[i].amigos[k], destino) == 0) {
                            printf("[!] Já são amigos.\n");
                            return;
                        }
                    }

                    strcpy(usuarios[i].amigos[usuarios[i].num_amigos++], destino);

                    strcpy(usuarios[j].amigos[usuarios[j].num_amigos++], origem);

                    salvar_todos_usuarios(usuarios, total);
                    printf("[!] Amizade adicionada com sucesso!\n");
                    return;
                }
            }
        }
    }
    printf("[X] Um dos usuários não foi encontrado.\n");
}

/* -----------------------------------------------
 Ver perfil do usuário logado
 ----------------------------------------------- */
void ver_perfil(char *username) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);
    char a;

    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, username) == 0) {
            printf("\n------------------------------------------\n");
            printf("              Perfil de @%s\n", username);
            printf("------------------------------------------\n");
            printf("Gosto musical: %s\n", usuarios[i].musica);
            printf("Hobbie: %s\n", usuarios[i].hobbie);
            printf("Número de amigos: %d\n", usuarios[i].num_amigos);
            printf("------------------------------------------\n\n");
            //printf("Digite qualquer coisa para sair de <Ver Meu perfil>: ");
            //scanf("%c", &a);
            //for(int i = 0; i < 999999; i++)
            return;
        }
    }

    printf("[X] Usuário não encontrado.\n");
}

/* -----------------------------------------------
 Mostra todos os amigos do usuário logado
 ----------------------------------------------- */
void listar_amigos(char *username) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, username) == 0) {
            printf("\n------------------------------------------\n");
            printf("        Lista de amigos de @%s\n", username);
            printf("------------------------------------------\n");

            if (usuarios[i].num_amigos == 0) {
                printf("Você ainda não tem amigos adicionados.\n");
            } else {
                for (int j = 0; j < usuarios[i].num_amigos; j++) {
                    printf("- %s\n", usuarios[i].amigos[j]);
                }
            }

            printf("------------------------------------------\n\n");
            return;
        }
    }

    printf("[X] Usuário não encontrado.\n");
}

void sugerir_amigos_por_interesse(char *username) {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    int i, j;
    int index_user = -1;

    // Encontra o usuário atual
    for (i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, username) == 0) {
            index_user = i;
            break;
        }
    }

    if (index_user == -1) {
        printf("[X] Usuário não encontrado.\n");
        return;
    }

    Usuario atual = usuarios[index_user];

    printf("\n------------------------------------------\n");
    printf("Sugestões de amizade para @%s\n", username);
    printf("------------------------------------------\n");

    int sugeridos = 0;

    for (i = 0; i < total; i++) {
        if (i == index_user)
            continue;

        // Verifica se já são amigos
        int ja_sao_amigos = 0;
        for (j = 0; j < atual.num_amigos; j++) {
            if (strcmp(atual.amigos[j], usuarios[i].username) == 0) {
                ja_sao_amigos = 1;
                break;
            }
        }

        if (ja_sao_amigos)
            continue;

        // Verifica interesse em comum
        if (strcmp(atual.hobbie, usuarios[i].hobbie) == 0 ||
            strcmp(atual.musica, usuarios[i].musica) == 0) {

            printf("- %s (interesse em comum: ", usuarios[i].username);
            if (strcmp(atual.hobbie, usuarios[i].hobbie) == 0)
                printf("hobbie");
            if (strcmp(atual.hobbie, usuarios[i].hobbie) == 0 && strcmp(atual.musica, usuarios[i].musica) == 0)
                printf(" e ");
            if (strcmp(atual.musica, usuarios[i].musica) == 0)
                printf("música");
            printf(")\n");
            sugeridos++;
        }
    }

    if (sugeridos == 0) {
        printf("Nenhuma sugestão com base em interesses em comum.\n");
    }

    printf("------------------------------------------\n\n");
}



/* -----------------------------------------------
 menu principial da rede, quando o usuário fizer
 o login correcto na rede
 ----------------------------------------------- */
void logado(char *username) {
    char op2;
    char amigo[50];

    do 
    {   
        printf("------------------------------------------\n");
        printf("         Olá, @%s \n", username);
        printf("------------------------------------------\n\n");

        printf("------------------------------------------\n");
        printf("|           [1] Alterar senha            |\n");
        printf("------------------------------------------\n");
        printf("|            [2] Ver perfil              |\n");
        printf("------------------------------------------\n");
        printf("|            [3] Meus amigos             |\n");
        printf("------------------------------------------\n");
        printf("|          [4] Adicionar amigo           |\n");
        printf("------------------------------------------\n");
        printf("|         [5] Sugestão de Amizade        |\n");
        printf("------------------------------------------\n");
        printf("|            [6] Modo chat               |\n");
        printf("------------------------------------------\n");
        printf("|               [0] Logout               |\n");
        printf("------------------------------------------\n");
        scanf(" %c", &op2);

        switch(op2) 
        {
            case '1':
                alterar_senha(username);
            break;

            case '2':
                ver_perfil(username);
            break;
            case '3':
                listar_amigos(username);
            break;

            case '4':
                printf("[?] Insira o nome de usuário para adicionar como amigo: ");
                scanf("%s", amigo);
                
                Usuario usuarios[MAX_USUARIOS];
                int total = carregar_usuarios(usuarios, MAX_USUARIOS);

                adicionar_amigo(usuarios, total, username, amigo);
            break;
            case '5':
                sugerir_amigos_por_interesse(username);
             break;
            case '6':
                printf("Iniciar chat com: ");
                scanf("%s", amigo);
                modo_chat(username, amigo);
            break;


            case '0':
                printf("Fazendo logout...\n");
            break;

            default:
                printf("Opção Inválida!\n");
        }
        //system("pause");
        //system("clear");
    } while(op2 != '0');
}

/* -----------------------------------------------
 Menu para fazer login na rede
 ----------------------------------------------- */
void fazer_login() {
    char username[50], senha[50];

    printf("------------------------------------------\n");
    printf("   Insira as suas informações de Login   \n");
    printf("------------------------------------------\n\n");

    
    printf("[?] Usuário: ");
    scanf("%s", username);
    printf("[?] Senha: ");
    fflush(stdin);
    scanf("%s", senha);
    fflush(stdin);
    system("clear");
    
    if (login(username, senha)) {
        /*printf("[!] Login realizado com sucesso! Bem-vindo, %s.\n", username);
        system("pause");
        system("cls");*/
        logado(username);
    } else {
        printf("[X] Usuário ou senha incorretos.\n");
    }
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// void modo_chat(char *user, char *amigo) {
//     char filename[100];
//     sprintf(filename, "chat_%s_%s.txt", user, amigo);

//     char input[256];
//     long last_pos = 0;

//     printf("Entrando no chat com @%s. Escreva '/sair' para sair.\n", amigo);

//     while (1) {
//         // Mostra novas mensagens
//         FILE *file = fopen(filename, "r");
//         if (file) {
//             fseek(file, last_pos, SEEK_SET);
//             char linha[300];
//             while (fgets(linha, sizeof(linha), file)) {
//                 printf("%s", linha);
//             }
//             last_pos = ftell(file);
//             fclose(file);
//         }

//         // Input do usuário
//         printf("> ");
//         fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = 0; // remove newline

//         if (strcmp(input, "/sair") == 0)
//             break;

//         // Escreve mensagem no arquivo
//         file = fopen(filename, "a");
//         if (file) {
//             fprintf(file, "@%s: %s\n", user, input);
//             fclose(file);
//         }

//         sleep(1); // Evita uso intensivo de CPU
//     }
// }


