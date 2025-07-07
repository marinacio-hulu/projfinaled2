#include "usuario.h"

Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;


int main() {
	setlocale(LC_ALL, "Portuguese");
    system("cls");
    atualizar_dados_usuarios(usuarios, &totalUsuarios);
    menu(usuarios, &totalUsuarios);
    return 0;
}
