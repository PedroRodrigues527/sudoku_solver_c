#include "menu.h"
#include "sudoku.h"

#define MAXLINE 512

int updateRoom(room, sendline)
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
        if(sendline[0] == '1' && strlen(sendline) == 2)
        {
            room = 2;
        }
        else
        {
            room = 1;
        }
        break;
    case 2:
        if(sendline[0] == 'F' && sendline[1] == 'F' && strlen(sendline) == 3)
        {
            //desistir o cliente
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
                exit(1);
            }
        }
        room = 2;
        break;
    default:
        break;
    }

    return room;
}

int responseLine(room, sendline, textuser, sudokuresolver, fullsudoku, points)
int room;
char sendline[MAXLINE];
char textuser[MAXLINE];
int sudokuresolver[9][9];
int fullsudoku[9][9];
int points;
{
    //char resultline[MAXLINE];
    strcpy(sendline, "");
    switch (room)
    {
    case 0:
        if(strlen(textuser) == 1) //Se nao tem opcao
        {
            strcat (sendline,"Indo ao Menu Principal...\n"); //Se der apenas ENTER Mostra o menu ao utilizador
        }
        else
        {
            strcat (sendline,"...\n"); //Volta à mensagem "CARREGUE EM 'ENTER'\n\n" caso o utilizador inserir mais do que o um carater exeto \0
        }
        break;
    case 1:
        if(textuser[0] == '2' && strlen(textuser) == 2)
        {
            strcat (sendline,"Escolheu opcao 2...\n");
            //print the line
            //strcat (sendline,sendline);
        }
        else if(textuser[0] == '1' && strlen(textuser) == 2)
        {
            strcat (sendline,"Escolheu opcao 1...\n");
        }
        else
        {
            strcat (sendline,"Nao escolheu nenhuma das opcoes...\n");
        }
        break;
    case 2:
        //1x1 1
        if(textuser[1] == 'x' && textuser[3] == ' ' && strlen(textuser) == 6)
        {
            if(isdigit(textuser[0]) && isdigit(textuser[2]) && isdigit(textuser[4]) && textuser[0] != 0 && textuser[2] != 0 && textuser[4] != 0)
            {
                // Subtrai 48 porque os digitos na tabela ASCII estão entre 48 e 57, inclusive
                int numlinha = (int)textuser[0] - 48;
                int numcoluna = (int)textuser[2] - 48;
                int valor = (int)textuser[4] - 48;

                //funcao de validar os dados no sudoku
                //validarsudoku(numlinha, numcoluna, valor, sudokuantes);
                points = verifyBoard(sendline, sudokuresolver, fullsudoku, numlinha, numcoluna, valor, points);
            }
            else
            {
                strcat (sendline,"Os valores inseridos são invalidos! Insere digitos de acordo com o formato pretendido...\n");
            }
            print(9, sudokuresolver);
            char buffer[MAXLINE];
            sprintf(buffer, "Pontos: %d\n", points);
            strcat (sendline,buffer);
        }
        else if(textuser[0] == 'F' && textuser[1] == 'F' && strlen(textuser) == 3)
        {
            strcat (sendline,"Desistindo do Jogo...\n");
            //mais dados de desistencia
        }
        else
        {
            strcat (sendline,"Formato/Opcao desconhecido(a)...\n");
            print(9, sudokuresolver);
            char buffer[MAXLINE];
            sprintf(buffer, "Pontos: %d\n", points);
            strcat (sendline,buffer);
        }
        break;
    default:
        strcat (sendline,"Isto nao devia acontecer...\n");
        break;
    }
    
    printf("Servidor: %s\n",sendline);
    return points;
}
