#include "usuarios.h"

NoBST* inserir_bst(NoBST *raiz, Usuario u) {
    if (raiz == NULL) {
        NoBST *novo = (NoBST*)malloc(sizeof(NoBST));
        novo->u = u;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(u.username, raiz->u.username) < 0)
        raiz->esq = inserir_bst(raiz->esq, u);
    else
        raiz->dir = inserir_bst(raiz->dir, u);

    return raiz;
}

void imprimir_usuarios_em_ordem(NoBST *raiz) {
    if (raiz != NULL) {
        imprimir_usuarios_em_ordem(raiz->esq);
        printf("- %s (%d amigos, Hobbie: %s, Música: %s)\n",
               raiz->u.username,
               raiz->u.num_amigos,
               raiz->u.hobbie,
               raiz->u.musica);
        imprimir_usuarios_em_ordem(raiz->dir);
    }
}

void ver_usuarios_em_ordem() {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    NoBST *raiz = NULL;
    for (int i = 0; i < total; i++) {
        raiz = inserir_bst(raiz, usuarios[i]);
    }

    printf("\n--- Usuários em ordem alfabética ---\n");
    imprimir_usuarios_em_ordem(raiz);
    printf("-------------------------------------\n");
}