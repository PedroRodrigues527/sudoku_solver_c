#include <menu.h>

int menu(room, sendline)
int room;
char *sendline;
{
    switch (room)
    {
    case 0:
        if(strlen(sendline) == 1) //Se nao tem opcao
        {
            room = 1; //Se der apenas ENTER Mostra o menu ao utilizador
        }
        else
        {
            room = 0; //Volta à mensagem "CARREGUE EM 'ENTER'\n\n" caso o utilizador inserir mais do que o um carater exeto \0
        }
        break;
    case 1:
        if(sendline[0] == '2' && strlen(sendline) == 2)
        {
            printf("Escolheu opcao 2...\n\n");
            
            //print the line
            printf("%s", sendline);

            room = 1;
        }
        else if(sendline[0] == '1' && strlen(sendline) == 2)
        {
            printf("Escolheu opcao 1...\n\n");
            room = 1;
        }
        else
        {
            printf("Nao escolheu nenhuma das opcoes...\n\n");
            room = 1;
        }
        break;
    
    default:
        printf("Isto nao devia acontecer...\n\n");
        break;
    }

    switch (room)
    {
    case 0:
        printf("CARREGUE EM 'ENTER'\n\n");
        break;
    case 1:
        printf("MENU PRINCIPAL\n");
        printf("1) Entrar no Sudoku\n");
        printf("2) Receber log\n\n");
        break;
    default:
        break;
    }

    return room;
}