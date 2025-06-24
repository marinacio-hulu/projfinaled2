#include "usuario.h"

CRITICAL_SECTION cs;
Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;
time_t ultimaModificacao = 0;

unsigned __stdcall atualizar_usuarios(void *arg) {
    while (1) {
        Sleep(1000);

        struct _stat st;
        if (_stat(FILENAME, &st) == 0) {
            if (st.st_mtime != ultimaModificacao) {
                EnterCriticalSection(&cs);
                carregar_usuarios(usuarios, &totalUsuarios);
                ultimaModificacao = st.st_mtime;
                LeaveCriticalSection(&cs);
            }
        }
    }
    return 0;
}

int main() {
    system("cls");
    InitializeCriticalSection(&cs);

    EnterCriticalSection(&cs);
    carregar_usuarios(usuarios, &totalUsuarios);
    struct _stat st;
    if (_stat(FILENAME, &st) == 0) {
        ultimaModificacao = st.st_mtime;
    }
    LeaveCriticalSection(&cs);

    _beginthreadex(NULL, 0, atualizar_usuarios, NULL, 0, NULL);

    menu(usuarios, &totalUsuarios);

    DeleteCriticalSection(&cs);
    return 0;
}
