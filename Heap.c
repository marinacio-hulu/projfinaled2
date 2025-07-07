#include "usuarios.h"

void heap_insert(MaxHeap *heap, Usuario u) {
    int i = heap->tamanho++;
    while (i > 0 && u.num_amigos > heap->dados[(i - 1) / 2].num_amigos) {
        heap->dados[i] = heap->dados[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->dados[i] = u;
}

Usuario heap_pop(MaxHeap *heap) {
    Usuario topo = heap->dados[0];
    Usuario temp = heap->dados[--heap->tamanho];

    int i = 0, filho;
    while (2 * i + 1 < heap->tamanho) {
        filho = 2 * i + 1;
        if (filho + 1 < heap->tamanho &&
            heap->dados[filho + 1].num_amigos > heap->dados[filho].num_amigos)
            filho++;

        if (temp.num_amigos >= heap->dados[filho].num_amigos)
            break;

        heap->dados[i] = heap->dados[filho];
        i = filho;
    }
    heap->dados[i] = temp;
    return topo;
}

void exibir_ranking_usuarios() {
    Usuario usuarios[MAX_USUARIOS];
    int total = carregar_usuarios(usuarios, MAX_USUARIOS);

    MaxHeap heap = {.tamanho = 0};
    for (int i = 0; i < total; i++) {
        heap_insert(&heap, usuarios[i]);
    }

    printf("\n--- Ranking de usuários com mais amigos ---\n");
    for (int i = 0; i < total && i < 10; i++) {
        Usuario top = heap_pop(&heap);
        printf("%d. %s (%d amigos)\n", i + 1, top.username, top.num_amigos);
    }
    printf("--------------------------------------------\n");
}