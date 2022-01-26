#include <menu.h>

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

void responseLine(room, sendline, textuser)
int room;
char sendline[MAXLINE];
char textuser[MAXLINE];
{
    //char resultline[MAXLINE];
    //strcpy (resultline,"");
    switch (room)
    {
    case 0:
        if(strlen(textuser) == 1) //Se nao tem opcao
        {
            strcat (sendline,"Indo ao Menu Principal...\0"); //Se der apenas ENTER Mostra o menu ao utilizador
        }
        else
        {
            strcat (sendline," ... \0"); //Volta à mensagem "CARREGUE EM 'ENTER'\n\n" caso o utilizador inserir mais do que o um carater exeto \0
        }
        break;
    case 1:
        if(textuser[0] == '2' && strlen(textuser) == 2)
        {
            strcat (sendline,"Escolheu opcao 2...\0");
            //print the line
            //strcat (sendline,sendline);
        }
        else if(textuser[0] == '1' && strlen(textuser) == 2)
        {
            strcat (sendline,"Escolheu opcao 1...\0");
        }
        else
        {
            strcat (sendline,"Nao escolheu nenhuma das opcoes...\0");
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
            }
            else
            {
                strcat (sendline,"Os valores inseridos são invalidos! Insere digitos de acordo com o formato pretendido.\0");
            }
        }
        else if(textuser[0] == 'F' && textuser[1] == 'F' && strlen(textuser) == 3)
        {
            //alguem desistiu
        }
        else
        {
            strcat (sendline,"Formato/Opcao desconhecido(a)...\0");
        }
        break;
    default:
        strcat (sendline,"Isto nao devia acontecer...\0");
        break;
    }
    
}
