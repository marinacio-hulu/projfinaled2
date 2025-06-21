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
    printf("\t\t\t+----------------------------------------------+\n");
    printf("\t\t\t| [%c] %-40s |\n", icone, msg);
    printf("\t\t\t+----------------------------------------------+\n");
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
    char username[MAX_NOME];
    char senha[MAX_SENHA];

    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|               CRIAR CONTA              |\n");
    printf("\t\t\t+----------------------------------------+\n\n");

    printf("\t\t\t[?] Nome de usuário: ");
    scanf(" %s", username);

    str_tolower(username);
    strcpy(novo.username, username);
    
    if (encontrar_usuario(usuarios, *totalUsuarios, novo.username) != -1) {
        msg('!', "Usuário já existe ou inválido");
        return;
    }

    printf("\t\t\t[?] Senha: ");
    scanf(" %s", senha);
    hash_senha(senha, novo.senha);

    printf("\t\t\t[?] Quantas características? ");
    scanf("%d", &novo.numCaracteristicas);

    for (int i = 0; i < novo.numCaracteristicas; i++) {
        printf("\t\t\t[?] Característica %d: ", i+1);
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
    char username[MAX_NOME], senha[MAX_SENHA], senha_hash[MAX_SENHA];

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

    if(strcmp(remetente->username, destino) == 0){
        msg('x', "Você não pode adicionar você");
        return;
    }

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
 Função para sugerir amigos com base nas
 características em comum
 ----------------------------------------------- */
void sugerir_amigos(Usuario usuarios[], int total, Usuario *usuario) {
    int i, j, k;
    int sugeridos = 0;

    /*printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|         SUGESTÕES DE AMIZADE           |\n");
    printf("\t\t\t+----------------------------------------+\n\n");*/

    for (i=0; i<total; i++) {
        Usuario *outro = &usuarios[i];

        if (!outro->ativo || strcmp(outro->username, usuario->username) == 0)
            continue;

        
        int amigos = 0;
        for (j=0; j<usuario->numAmigos; j++) {
            if (strcmp(usuario->amigos[j], outro->username) == 0) {
                amigos = 1;
                break;
            }
        }
        if (amigos) continue;

        
        int comum = 0;
        char caracteristicasComum[MAX_CARACTERISTICAS][MAX_CARACTERISTICA];
        for (j=0; j<usuario->numCaracteristicas; j++) {
            for (k=0; k<outro->numCaracteristicas; k++) {
                if (strcmp(usuario->caracteristicas[j], outro->caracteristicas[k]) == 0) {
                    strcpy(caracteristicasComum[comum], outro->caracteristicas[k]);
                    comum++;
                    
                }
            }
        }

        if (comum > 0) {
            sugeridos++;
            printf("\t\t\t+----------------------------------------+\n");
            printf("\t\t\t| @%-20s                  |\n", outro->username);
            printf("\t\t\t+----------------------------------------+\n");
            printf("\t\t\t| (%-2d caracteristica(s) em comum)        |\n", comum);
            printf("\t\t\t+----------------------------------------+\n");
            int l;
            for (l=0; l<comum; l++) {
                printf("\t\t\t| %-30s         |\n", caracteristicasComum[l]);
                printf("\t\t\t+----------------------------------------+\n");
            }
        }

        system("pause");
        system("cls");
    }

    if (sugeridos == 0) {
        msg('!', "Nenhuma sugestão encontrada.");
    } else {
        printf("\n\t\t\t[!] Total de sugestões: %d\n", sugeridos);
    }
}

/* -----------------------------------------------
 Função para mostrar todos os amigos do usuário
 ----------------------------------------------- */
void meus_amigos(Usuario usuarios[], int total, Usuario *usuario) {

    if (usuario->numAmigos == 0) {
        msg('!', "Você ainda não tem amigos.");
        return;
    }
    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|              MEUS AMIGOS               |\n");
    printf("\t\t\t+----------------------------------------+\n");
    int i;
    for (i=0; i<usuario->numAmigos; i++) {
        char *amigo = usuario->amigos[i];
        printf("\t\t\t| @%-20s                  |\n", amigo);
        printf("\t\t\t+----------------------------------------+\n");
        
    }
    printf("\t\t\t| Total de amigos: %-3d                   |\n", usuario->numAmigos);
    printf("\t\t\t+----------------------------------------+\n");

}

/* -----------------------------------------------
 Função para trocar a senha do usuario 
----------------------------------------------- */
void trocar_senha(Usuario usuarios[], int total, Usuario *usuario){
    char senha[MAX_SENHA], senha_hash[MAX_SENHA];

    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|               TROCAR SENHA             |\n");
    printf("\t\t\t+----------------------------------------+\n\n");

    printf("\t\t\t[?] Senha: ");
    scanf(" %s", senha);
    hash_senha(senha, senha_hash);

    if(strcmp(usuario->senha, senha_hash) == 0){
        printf("\t\t\t[?] Insira a nova senha: ");
        scanf(" %s", senha);
        hash_senha(senha, senha_hash);

        strcpy(usuario->senha, senha_hash);
        salvar_usuarios(usuarios, total);
        msg('!', "Senha trocada!");
    } else {
        msg('x', "Senha incorrecta");
    }

}

/* -----------------------------------------------
 Função para remover amigo do usuário
 ----------------------------------------------- */
void remover_amigo(Usuario usuarios[], int total, Usuario *usuario) {
    if (usuario->numAmigos == 0) {
        msg('!', "Sem amigos para remover.");
        return;
    }

    char nome[MAX_NOME];
    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|             REMOVER AMIGO              |\n");
    printf("\t\t\t+----------------------------------------+\n\n");
    printf("\t\t\t[?] Nome do amigo a remover: ");
    scanf(" %s", nome);
    str_tolower(nome);

    int idxAmigo = encontrar_usuario(usuarios, total, nome);
    if (idxAmigo == -1) {
        msg('x', "Amigo não encontrado.");
        return;
    }

    int i,j, encontrou = 0;
    for (i = 0; i < usuario->numAmigos; i++) {
        if (strcmp(usuario->amigos[i], nome) == 0) {
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        msg('x', "Esse usuário não é seu amigo.");
        return;
    }

    // aqui removo o amigo da lista do usuário
    for (j=i; j < usuario->numAmigos - 1; j++) {
        strcpy(usuario->amigos[j], usuario->amigos[j + 1]);
    }
    usuario->numAmigos--;

    // aq tou a remover o usuário da lista do amigo
    Usuario *amigo = &usuarios[idxAmigo];
    for (i=0; i<amigo->numAmigos; i++) {
        if (strcmp(amigo->amigos[i], usuario->username) == 0) {
            for (j=i; j<amigo->numAmigos - 1; j++) {
                strcpy(amigo->amigos[j], amigo->amigos[j + 1]);
            }
            amigo->numAmigos--;
            break;
        }
    }

    msg('!', "Amigo removido com sucesso.");
    salvar_usuarios(usuarios, total);
}


/* -----------------------------------------------
 Função para seguir ou deixar de seguir um usuario
 ----------------------------------------------- */
void seguir_usuario(Usuario usuarios[], int total, Usuario *usuario) {
    char nome[MAX_NOME];
    int i, j;
    printf("\t\t\t+----------------------------------------+\n");
    printf("\t\t\t|               SEGUIR USUÁRIO           |\n");
    printf("\t\t\t+----------------------------------------+\n\n");
    printf("\t\t\t[?] Username que deseja seguir: ");
    scanf(" %s", nome);
    str_tolower(nome);

    if (strcmp(nome, usuario->username) == 0) {
        msg('x', "Você não pode seguir você.");
        return;
    }

    int idx = encontrar_usuario(usuarios, total, nome);
    if (idx == -1 || !usuarios[idx].ativo) {
        msg('x', "Usuário não encontrado ou inativo.");
        return;
    }

    Usuario *alvo = &usuarios[idx];

    for (i=0; i<usuario->seguindo; i++) {
        if (strcmp(usuario->seguindoUsuarios[i], nome) == 0) {
            char op;
            printf("\t\t\t[!] Você já segue esse usuário.\n");
            printf("\t\t\t[?] Deseja deixar de seguir? (s/n): ");
            scanf(" %c", &op);

            
            if (op == 's' || op == 'S') {
                // aq tou a remover da lista seguindo do usuário
                for (j=i; j<usuario->seguindo - 1; j++) {
                    strcpy(usuario->seguindoUsuarios[j], usuario->seguindoUsuarios[j + 1]);
                }
                usuario->seguindo--;

                // removo a qty de seguidores do usuário que deixei de seguir
                if (alvo->seguidores > 0) {
                    alvo->seguidores--;
                }

                msg('!', "Deixou de seguir este usuário");
                salvar_usuarios(usuarios, total);
            } else {
                msg('!', "Operação cancelada.");
            }
            return;
        }
    }

    strcpy(usuario->seguindoUsuarios[usuario->seguindo++], nome);
    alvo->seguidores++;

    msg('!', "Começou a seguir este usuário!");
    salvar_usuarios(usuarios, total);
}

/* -----------------------------------------------
 Função para mostrar grau de popularidade
 ----------------------------------------------- */
void grau_popularidade(Usuario *usuario) {
    printf("\t\t\t+------------------------------------------+\n");
    printf("\t\t\t|            GRAU DE POPULARIDADE          |\n");
    printf("\t\t\t+------------------------------------------+\n");
    printf("\t\t\t| Username: @%-20s          |\n", usuario->username);
    printf("\t\t\t|------------------------------------------+\n");
    printf("\t\t\t| Seguidores: %-3d                          |\n", usuario->seguidores);
    printf("\t\t\t|------------------------------------------+\n");
    printf("\t\t\t| Seguindo: %-3d                            |\n", usuario->seguindo);
    printf("\t\t\t+------------------------------------------+\n\n");
    
}

/* -----------------------------------------------
 Função para mostrar top 3 influencers
 ----------------------------------------------- */
void mostrar_influencers(Usuario usuarios[], int total) {
    int influencia[MAX_USUARIOS], i,j;

    for (i=0; i<total; i++) {
        influencia[i] = usuarios[i].seguidores + usuarios[i].numAmigos;
    }

    printf("\t\t\t+-----------------------------------------------------------------------------+\n");
    printf("\t\t\t|                             TOP 3 INFLUENCERS                               |\n");
    printf("\t\t\t+---+-----------------------+-----------------+-------------+-----------------+\n");

    for (j=0; j<3; j++) {
        int max = -1;
        int idx = -1;

        for (i=0; i<total; i++) {
            if (influencia[i] > max) {
                max = influencia[i];
                idx = i;
            }
        }

        if (idx != -1 && max > 0) {
            printf("\t\t\t| %d | @%-20s | Influência: %-3d | Amigos: %-3d | Seguidores: %-3d |\n", 
                j+1, usuarios[idx].username, max, usuarios[idx].numAmigos, usuarios[idx].seguidores);
            printf("\t\t\t+---+-----------------------+-----------------+-------------+-----------------+\n");
            
            influencia[idx] = -1;
        }
    }
}

/* -----------------------------------------------
 Função para encontrar amigos em comum entre dois usuários
 ----------------------------------------------- */
void amigos_em_comum(Usuario usuarios[], int total) {
    char user1[MAX_NOME], user2[MAX_NOME];
    printf("\t\t\t[?] Primeiro usuário: ");
    scanf(" %s", user1);
    str_tolower(user1);

    printf("\t\t\t[?] Segundo usuário: ");
    scanf(" %s", user2);
    str_tolower(user2);

    system("cls");

    int idx1 = encontrar_usuario(usuarios, total, user1);
    int idx2 = encontrar_usuario(usuarios, total, user2);

    if (idx1 == -1 || idx2 == -1) {
        msg('x', "Um ou ambos usuários não encontrados.");
        return;
    }

    Usuario *u1 = &usuarios[idx1];
    Usuario *u2 = &usuarios[idx2];
    printf("\t\t\t+----------------------------+\n");
    printf("\t\t\t| Amigos em comum entre Eles |\n");
    printf("\t\t\t+----------------------------+\n");
    int encontrados = 0, i, j;
    for (i=0; i<u1->numAmigos; i++) {
        for (j=0; j<u2->numAmigos; j++) {
            if (strcmp(u1->amigos[i], u2->amigos[j]) == 0) {
                printf("\t\t\t| @%-20s      |\n", u1->amigos[i]);
                printf("\t\t\t+----------------------------+\n");
                encontrados++;
            }
        }
    }
    if (encontrados == 0) {
        msg('!', "Nenhum amigo em comum encontrado.");
    }
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
    char user[20];

    do {
        printf("\t\t+---------------------------------------------------------------------------------+\n");
        printf("\t\t|                                 C O N E C T - C                                 |\n");
        printf("\t\t+---------------------------------------------------------------------------------+\n");
        printf("\t\t| Seja Bem Vindo, @%-20s                                           |\n", usuario->username);
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 1 | Ver Perfil                         | 8 | Remover Amigo                      |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 2 | Procurar Usuário                   | 9 | Seguir Usuário                     |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 3 | Editar Perfil                      | A | Grau de Popularidade               |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 4 | Enviar Pedido de Amizade           | B | Top 3 Influencers                  |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 5 | Ver Pedidos de Amizade             | C | Amigos em Comum                    |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 6 | Sugestão de Amigos                 | D | Distância Mínima                   |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 7 | Meus Amigos                        | E | Enviar Mensagem                    |\n");
        printf("\t\t+---+------------------------------------+---+------------------------------------+\n");
        printf("\t\t| 0 | Terminar sessão                                                             |\n");
        printf("\t\t+---+-----------------------------------------------------------------------------+\n");
        printf("\t\t");
        scanf(" %c", &op2);
        system("cls");
        
        if (op2=='1') {
            mostrar_perfil(usuario);
        } else if(op2=='2') {
            printf("\t\t\t+----------------------------------------+\n");
            printf("\t\t\t|            PROCURAR USUÁRIO            |\n");
            printf("\t\t\t+----------------------------------------+\n\n");
            printf("\t\t\t[?] Insira o username: ");
            scanf("%s", user);
            system("cls");
            
            str_tolower(user);

            int idx = encontrar_usuario(usuarios, totalUsuarios, user);
            if (idx != -1 && strcmp(usuarios[idx].username, user) == 0) {
                mostrar_perfil(&usuarios[idx]);
            } else {
                msg('x', "Usuário não econtrado");
            }
        } else if(op2=='3'){
            editar_perfil(usuarios, usuario, totalUsuarios);
        } else if(op2=='4') {
            enviar_pedido_amizade(usuarios, totalUsuarios, usuario);
        } else if(op2=='5'){
            ver_pedidos_amizade(usuarios, totalUsuarios, usuario);
        } else if(op2=='6') {
            sugerir_amigos(usuarios, totalUsuarios, usuario);
        } else if(op2=='7'){
            meus_amigos(usuarios, totalUsuarios, usuario);
        } else if(op2=='8'){
            remover_amigo(usuarios, totalUsuarios, usuario);
        } else if(op2=='9'){
            seguir_usuario(usuarios, totalUsuarios, usuario);
        } else if(op2=='a' || op2 == 'A'){
            printf("\t\t\t[?] Insira o username: ");
            scanf("%s", user);
            system("cls");
            
            str_tolower(user);

            int idx = encontrar_usuario(usuarios, totalUsuarios, user);
            if (idx != -1 && strcmp(usuarios[idx].username, user) == 0) {
                grau_popularidade(&usuarios[idx]);
            } else {
                msg('x', "Usuário não econtrado");
            }
        } else if(op2=='b' || op2 == 'B'){
            mostrar_influencers(usuarios, totalUsuarios);
        } else if(op2=='c' || op2 == 'C'){
            amigos_em_comum(usuarios, totalUsuarios);
        } else if(op2=='d' || op2 == 'D'){

        } else if(op2=='e' || op2 == 'E'){

        } else if(op2=='0') {
            msg('!', "Secção terminada");
        } else {
            msg('x', "Opção inválida");
        }
        
        system("pause");
        system("cls");
    } while (op2 != '0');
}

/* -----------------------------------------------
 menu editar perfil 
----------------------------------------------- */
void editar_perfil(Usuario usuarios[], Usuario *usuario, int totalUsuarios) {
    char op3;
    do {
        printf("\t\t\t+----------------------------------------+\n");
        printf("\t\t\t|              EDITAR PERFIL             |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t| 1 | Trocar Senha                       |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        /*printf("\t\t\t| 2 | Apagar Conta                       |\n");
        printf("\t\t\t+---+------------------------------------+\n");*/
        printf("\t\t\t| 0 | Voltar                             |\n");
        printf("\t\t\t+---+------------------------------------+\n");
        printf("\t\t\t");
        scanf(" %c", &op3);
        system("cls");
        
        if (op3=='1') {
            trocar_senha(usuarios, totalUsuarios, usuario);
        } else if(op3=='2') {
            apagar_conta(usuarios, totalUsuarios, usuario);
        } else if(op3=='0') {
        } else {
            msg('x', "Opção inválida");
        }
        system("pause");
        system("cls");
    } while (op3 != '0');
}


/* -----------------------------------------------
 feature não 100% funcional 
----------------------------------------------- */
void apagar_conta(Usuario usuarios[], int total, Usuario *usuario){
    char op;
    printf("\t\t\t+--------------------------------------------+\n");
    printf("\t\t\t|                APAGAR CONTA                |\n");
    printf("\t\t\t+--------------------------------------------+\n");
    printf("\t\t\t[?] Deseja realmente apagar a sua conta?\n");
    printf("\t\t\tO seu username não poderá mais ser utilizado\n");
    printf("\t\t\tCaso pretenda criar uma nova conta\n");
    printf("\t\t\t(s/n): ");
    scanf(" %c", &op);

    if (op == 's' || op == 'S') {
        usuario->ativo = 0;
        msg('!', "A sua conta foi excluída");
        salvar_usuarios(usuarios, total);
        exit(0);
    } else {
        msg('!', "Operação cancelada.");
    }
    return;
}