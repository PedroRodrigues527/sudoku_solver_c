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

            room = 2;
        }
        else
        {
            printf("Nao escolheu nenhuma das opcoes...\n\n");
            room = 1;
        }
        break;
    case 2:
        //1x1 1
        if(sendline[1] == 'x' && sendline[3] == ' ' && strlen(sendline) == 6)
        {
            if(isdigit(sendline[0]) && isdigit(sendline[2]) && isdigit(sendline[4]) && sendline[0] != 0 && sendline[2] != 0 && sendline[4] != 0)
            {
                // Subtrai 48 porque os digitos na tabela ASCII estão entre 48 e 57, inclusive
                int numlinha = (int)sendline[0] - 48;
                int numcoluna = (int)sendline[2] - 48;
                int valor = (int)sendline[4] - 48;

                //funcao de validar os dados no sudoku
                
            }
            else
            {
                printf("Os valores inseridos são invalidos! Insere digitos de acordo com o formato pretendido.\n\n");
            }
        }
        else if(sendline[0] == 'F' && sendline[1] == 'F' && strlen(sendline) == 3)
        {
            //desistir o cliente
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
                exit(1);
            }
        }
        else
        {
            printf("Formato/Opcao nao encontrada...\n\n");
        }
        room = 2;
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
    case 2:
        printf("SUDOKU\n");
        //printf(arraydosudoku);
        printf("FORMATO: [Num Linha]x[Num Coluna] [Valor]\n");
        printf("Insire 'FF' e pressione 'Enter' para desistir do Sudoku\n");

        printf("\n");
        break;
    default:
        break;
    }

    return room;
}
