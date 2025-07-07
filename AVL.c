#include "usuarios.h"


int altura(NoAVL *n) {
    return n ? n->altura : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int fator_balanceamento(NoAVL *n) {
    return n ? altura(n->esq) - altura(n->dir) : 0;
}

NoAVL* rotacao_direita(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = 1 + max(altura(y->esq), altura(y->dir));
    x->altura = 1 + max(altura(x->esq), altura(x->dir));

    return x;
}

NoAVL* rotacao_esquerda(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = 1 + max(altura(x->esq), altura(x->dir));
    y->altura = 1 + max(altura(y->esq), altura(y->dir));

    return y;
}

NoAVL* inserir_avl(NoAVL *no, Usuario u) {
    if (no == NULL) {
        NoAVL *novo = malloc(sizeof(NoAVL));
        novo->u = u;
        novo->esq = novo->dir = NULL;
        novo->altura = 1;
        return novo;
    }

    if (strcmp(u.username, no->u.username) < 0)
        no->esq = inserir_avl(no->esq, u);
    else if (strcmp(u.username, no->u.username) > 0)
        no->dir = inserir_avl(no->dir, u);
    else
        return no; // usuário duplicado (mesmo nome)

    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    int fb = fator_balanceamento(no);

    // Rebalancear se necessário
    if (fb > 1 && strcmp(u.username, no->esq->u.username) < 0)
        return rotacao_direita(no);

    if (fb < -1 && strcmp(u.username, no->dir->u.username) > 0)
        return rotacao_esquerda(no);

    if (fb > 1 && strcmp(u.username, no->esq->u.username) > 0) {
        no->esq = rotacao_esquerda(no->esq);
        return rotacao_direita(no);
    }

    if (fb < -1 && strcmp(u.username, no->dir->u.username) < 0) {
        no->dir = rotacao_direita(no->dir);
        return rotacao_esquerda(no);
    }

    return no;
}


Usuario* buscar_avl(NoAVL *no, char *username) {
    if (no == NULL)
        return NULL;

    int cmp = strcmp(username, no->u.username);
    if (cmp == 0)
        return &no->u;
    else if (cmp < 0)
        return buscar_avl(no->esq, username);
    else
        return buscar_avl(no->dir, username);
}

void buscar_usuario_por_nome() {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    NoAVL *raiz = NULL;
    for (int i = 0; i < total; i++)
        raiz = inserir_avl(raiz, usuarios[i]);

    char nome_busca[50];
    printf("Digite o nome do usuário a buscar: ");
    scanf("%s", nome_busca);

    Usuario *u = buscar_avl(raiz, nome_busca);
    if (u) {
        printf("Usuário encontrado:\n");
        printf("- Nome: %s\n- Amigos: %d\n- Hobbie: %s\n- Música: %s\n",
               u->username, u->num_amigos, u->hobbie, u->musica);
    } else {
        printf("Usuário não encontrado.\n");
    }
}
