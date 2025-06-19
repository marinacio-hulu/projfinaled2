#include "usuario.h"

/* -----------------------------------------------
 Função auxiliar para converter tudo 
 para minusculo
 ----------------------------------------------- */
void str_tolower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

/* -----------------------------------------------
 Função para exibir uma mensagem
 ----------------------------------------------- */
void msg(char icone, char *msg){
    system("cls");
    printf("\n\n\n\n\n\n\n\n");
    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t| [%c] %-34s |\n", icone, msg);
    printf("\t\t\t+----------------------------------------+\n");
    printf("\n\n\n\n\n\n\n\n");
}

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
    fclose(f);
    return 1;
}

/* -----------------------------------------------
 Função para mostrar todos os usuários
 estou apenas a usar para alguns debugs**
 ----------------------------------------------- */
void mostrar_todos_usuarios(Usuario usuarios[], int *total) {
    int i;
    for(i=0; i<*total; i++){
        printf("---------------------\n");
        printf("User: %s\n", usuarios[i].username);
        printf("Pass: %s\n", usuarios[i].senha);
        printf("---------------------\n");

    }
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
    for (i=0; i<total; i++) {
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
    char username[20];
    char senha[20];

    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|               CRIAR CONTA              |\n");
    printf("\t\t\t+----------------------------------------+\n\n");

    printf("\t\t\t[?] Nome de usuário: ");
    scanf(" %s", username);

    str_tolower(username);
    strcpy(novo.username, username);
    
    if (encontrar_usuario(usuarios, *totalUsuarios, novo.username) != -1) {
        msg('!', "Usuário já existe.");
        return;
    }

    printf("\t\t\t[?] Senha: ");
    scanf(" %s", senha);
    hash_senha(senha, novo.senha);

    printf("\t\t\t[?] Quantas características? ");
    scanf("%d", &novo.numCaracteristicas);

    for (int i = 0; i < novo.numCaracteristicas; i++) {
        printf("\t\t\t[?] Característica %d: ", i + 1);
        scanf(" %s", novo.caracteristicas[i]);
    }

    system("cls");

    novo.numAmigos = 0;
    novo.seguidores = 0;
    novo.seguindo = 0;
    novo.numPedidos = 0;
    novo.ativo = 1;

    usuarios[*totalUsuarios] = novo;
    (*totalUsuarios)++;

    salvar_usuarios(usuarios, *totalUsuarios);
    msg('!', "Usuário cadastrado!");
}

/* -----------------------------------------------
 Função fazer o login na rede
 ----------------------------------------------- */
int login(Usuario usuarios[], int *totalUsuarios) {
    char username[20], senha[20], senha_hash[20];

    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|                 LOGIN                  |\n");
    printf("\t\t\t+----------------------------------------+\n\n");

    printf("\t\t\t[?] Username: ");
    scanf(" %s", username);
    printf("\t\t\t[?] Senha: ");
    scanf(" %s", senha);
    system("cls");

    str_tolower(username);
    hash_senha(senha, senha_hash);

    int idx = encontrar_usuario(usuarios, *totalUsuarios, username);
    if (idx != -1 && strcmp(usuarios[idx].senha, senha_hash) == 0) {
        msg('!', "Login bem-sucedido!");
        system("pause");
        system("cls");

        conectC(usuarios, &usuarios[idx], *totalUsuarios);
        return idx;
    }

    msg('x', "Username ou Senha incorrectos.");
    return -1;
}


/* -----------------------------------------------
Função para mostrar o perfil de um usuário
----------------------------------------------- */
void mostrar_perfil(Usuario *usuario) {
    printf("\t\t\t+------------------------------------------+\n");
    printf("\t\t\t|                 PERFIL                   |\n", usuario->username);
    printf("\t\t\t+------------------------------------------+\n");
    printf("\t\t\t| Username: @%-20s          |\n", usuario->username);
    printf("\t\t\t|------------------------------------------+\n");
    printf("\t\t\t| Caracteristicas: %-3d                     |\n", usuario->numCaracteristicas);
    printf("\t\t\t|------------------------------------------+\n");
    printf("\t\t\t| Amigos: %-3d                              |\n", usuario->numAmigos);
    printf("\t\t\t|------------------------------------------+\n");
    printf("\t\t\t| Seguidores: %-3d                          |\n", usuario->seguidores);
    printf("\t\t\t|------------------------------------------+\n");
    printf("\t\t\t| Seguindo: %-3d                            |\n", usuario->seguindo);
    printf("\t\t\t+------------------------------------------+\n\n");
}


/* -----------------------------------------------
 Função para enviar pedido de amizade
 ----------------------------------------------- */
void enviar_pedido_amizade(Usuario *usuarios, int total, Usuario *remetente) {
    char destino[MAX_NOME];
    int i;

    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|              FAZER PEDIDO              |\n");
    printf("\t\t\t+----------------------------------------+\n\n");

    printf("\t\t\t[?] Nome do usuário: ");
    scanf(" %s", destino);
    
    str_tolower(destino);
    int idx = encontrar_usuario(usuarios, total, destino);
    if (idx == -1) {
        msg('x', "Usuário não encontrado.");
        return;
    }
    
    Usuario *destinatario = &usuarios[idx];
    
    for (i=0; i<remetente->numAmigos; i++) {
        if (strcmp(remetente->amigos[i], destino) == 0) {
            msg('x', "Já são amigos.");
            return;
        }
    }
    
    for (i=0; i<destinatario->numPedidos; i++) {
        if (strcmp(destinatario->pedidos[i], remetente->username) == 0) {
            msg('!', "Pedido já enviado.");
            return;
        }
    }
    
    strcpy(destinatario->pedidos[destinatario->numPedidos++], remetente->username);
    msg('!', "Pedido envidado!");
    salvar_usuarios(usuarios, total);
}

/* -----------------------------------------------
 Função para ver os pedidos de amizade
 e aceitar ou rejeitar
 ----------------------------------------------- */
void ver_pedidos_amizade(Usuario *usuarios, int total, Usuario *usuario) {
    if (usuario->numPedidos == 0) {
        msg('!', "Nenhum pedido.");
        return;
    }
    
    int i;
    for (i= 0; i < usuario->numPedidos; i++) {
        char *solicitante = usuario->pedidos[i];
        printf("\t\t\t+----------------------------------------+\n");
        printf("\t\t\t| Pedido de: %-20s        |\n", solicitante);
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 1 | Aceitar                            |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 2 | Rejeitar                           |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t");
        char op;
        scanf(" %c", &op);
        
        int idxSolicitante = encontrar_usuario(usuarios, total, solicitante);
        if (idxSolicitante == -1) continue;
        
        if (op=='1') {
            
            strcpy(usuario->amigos[usuario->numAmigos++], solicitante);
            strcpy(usuarios[idxSolicitante].amigos[usuarios[idxSolicitante].numAmigos++], usuario->username);
            msg('!', "Pedido aceite");
        } else {
            msg('x', "Pedido rejeitado");
        }
        
        int j;
        for (j=i; j<usuario->numPedidos - 1; j++) {
            strcpy(usuario->pedidos[j], usuario->pedidos[j + 1]);
        }
        usuario->numPedidos--;
        i--;
    }
    
    salvar_usuarios(usuarios, total);
}

/* -----------------------------------------------
 menu 
----------------------------------------------- */
void menu(Usuario usuarios[], int *totalUsuarios) {
    char op;
    do {
        printf("\t\t\t+----------------------------------------+\n");
        printf("\t\t\t|             C O N E C T - C            |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 1 | Fazer Cadastro                     |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 2 | Fazer Login                        |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 0 | Sair                               |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t");
        scanf(" %c", &op);
        system("cls");
        
        if (op=='1') {
            cadastrar_usuario(usuarios, totalUsuarios);
        } else if(op=='2') {
            login(usuarios, totalUsuarios);
        } else if(op=='0') {
            exit(1);
        } else {
            msg('x', "Opção inválida");
        }
        system("pause");
        system("cls");
    } while (op != '0');
}

/* -----------------------------------------------
 submenu quando o usuário está logado
----------------------------------------------- */
void conectC(Usuario usuarios[], Usuario *usuario, int totalUsuarios) {
    char op2;
    do {
        printf("\t\t\t+----------------------------------------+\n");
        printf("\t\t\t|             C O N E C T - C            |\n");
        printf("\t\t\t+----------------------------------------+\n");
        printf("\t\t\t| Seja Bem Vindo, @%-20s  |\n", usuario->username);
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 1 | Ver Perfil                         |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 2 | Procurar Usuário                   |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 3 | Editar Perfil                      |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 4 | Enviar Pedido de Amizade           |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 5 | Ver Pedidos de Amizade             |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 0 | Terminar sessão                    |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t");
        scanf(" %c", &op2);
        system("cls");
        
        if (op2=='1') {
            mostrar_perfil(usuario);
        } else if(op2=='2') {
            char perfil[20];
            printf("\t\t\t+----------------------------------------+\n");
            printf("\t\t\t|            PROCURAR USUÁRIO            |\n");
            printf("\t\t\t+----------------------------------------+\n\n");
            printf("\t\t\t[?] Insira o username: ");
            scanf("%s", perfil);
            system("cls");
            
            str_tolower(perfil);

            int idx = encontrar_usuario(usuarios, totalUsuarios, perfil);
            if (idx != -1 && strcmp(usuarios[idx].username, perfil) == 0) {
                mostrar_perfil(&usuarios[idx]);
            } else {
                msg('x', "Usuário não econtrado");
            }
        } else if(op2=='3'){

        } else if(op2=='4') {
            enviar_pedido_amizade(usuarios, totalUsuarios, usuario);
        } else if(op2=='5'){
            ver_pedidos_amizade(usuarios, totalUsuarios, usuario);
        } else if(op2=='6') {

        } else if(op2=='7'){

        } else if(op2=='0') {
            msg('!', "Secção terminada");
        } else {
            msg('x', "Opção inválida");
        }
        
        system("pause");
        system("cls");
    } while (op2 != '0');
}