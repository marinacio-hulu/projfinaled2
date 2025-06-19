#include "usuario.h"

Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;

int main() {
    system("cls");
    carregar_usuarios(usuarios, &totalUsuarios);
    mostrar_todos_usuarios(usuarios, &totalUsuarios);
    menu(usuarios, &totalUsuarios);
    return 0;

}
