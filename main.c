#include "usuarios.h"
#include<sys/wait.h>

int main()
{
    char op1;

    printf("------------------------------------------\n");
    printf("        SIMULADOR DE REDE SOCIAL!!!\n");
    printf("------------------------------------------\n\n");

    do
    {
        printf("------------------------------------------\n");
        printf("|               [1] Login                |\n");
        printf("------------------------------------------\n");
        printf("|              [2] Sign Up               |\n");
        printf("------------------------------------------\n");
        printf("|               [0] Sair                 |\n");
        printf("------------------------------------------\n\n");
        scanf(" %c", &op1);
        system("clear");

        switch (op1)
        {
            case '1':
                fazer_login();
            break;

            case '2':
                cadastrar();
            break;

            case '0':
                printf("Saindo...\n");
            break;
        
            default:
                printf("Opção Inválida\n");
            break;
        }
        //usleep(500);
        //system("pause");
        system("clear");
        
    }while(op1 != '0');

    return 0;
}