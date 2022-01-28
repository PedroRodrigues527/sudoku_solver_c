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
                points = verifyBoard(sendline, sudokuresolver, fullsudoku, numlinha, numcoluna, valor, points);
            }
            else
            {
                strcat (sendline,"Os valores inseridos são invalidos! Insere digitos de acordo com o formato pretendido...\n");
            }
            print(9, sudokuresolver);
            //pontuacao
            char buffer[MAXLINE];
            sprintf(buffer, "Pontos: %d\n", points);
            strcat (sendline,buffer);
            //verifycomplete
        }
        else if(textuser[0] == 'F' && textuser[1] == 'F' && strlen(textuser) == 3)
        {
            strcat (sendline,"Desistindo do Jogo...\n");
            //atualizar dados de desistencia
            //trinco fechar
            updateNumberDesistencias();
            updateNumberClients(0);
            //trinco abrir
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
    
    printf("Servidor %d: %s\n", getpid(), sendline);
    return points;
}

int responseLineClient(room, sendline, textuser)
int room;
char sendline[MAXLINE];
char textuser[MAXLINE];
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
            FILE *the_file;
            the_file = fopen("dados", "r");
            if(the_file == NULL)
            {
                perror("Unable to open the file 'dados'!");
                exit(1);
            }
            char line[MAXLINE];
            //primeiralinha
            fgets(line, MAXLINE, the_file);
            strcat(sendline,"Numero de Clientes: ");
            strcat(sendline,line);
            //segundalinha
            fgets(line, MAXLINE, the_file);
            strcat(sendline,"Numero de Desistencias: ");
            strcat(sendline,line);
            //terceiralinha
            fgets(line, MAXLINE, the_file);
            strcat(sendline,"Pontos total: ");
            strcat(sendline,line);
            //quartalinha
            fgets(line, MAXLINE, the_file);
            strcat(sendline,"Numero de tentativas: ");
            strcat(sendline,line);


            strcat(sendline,"\n");

            fclose(the_file);
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
                strcat (sendline,"Ver resposta do servidor...\n");
            }
            else
            {
                strcat (sendline,"Os valores inseridos são invalidos! Insere digitos de acordo com o formato pretendido...\n");
            }
        }
        else if(textuser[0] == 'F' && textuser[1] == 'F' && strlen(textuser) == 3)
        {
            strcat (sendline,"Desistindo do Jogo...\n");
        }
        else
        {
            strcat (sendline,"Formato/Opcao desconhecido(a)...\n");
        }
        break;
    default:
        strcat (sendline,"Isto nao devia acontecer...\n");
        break;
    }
    
    printf("%s\n",sendline);
}

int updateNumberClients(int isIncrem)
{
    FILE *the_file;
    FILE *temp;
	the_file = fopen("dados", "r");
    temp = fopen("replace.tmp","w");
	if(the_file == NULL || temp == NULL)
	{
		perror("Unable to open the file 'dados' or 'temp.tmp'!");
		exit(1);
	}
	char line[MAXLINE];
	fgets(line, MAXLINE, the_file);

	//incrementar +1 ou -1
    int nclients;
    if(isIncrem == 1)
    {
	    nclients = atoi(line) + 1;
    }
    else
    {
        nclients = atoi(line) - 1;
    }

	fseek(the_file, 0, SEEK_SET);
    int count = 0;
    while ((fgets(line, MAXLINE, the_file)) != NULL)
    {
        count++;

        /* If current line is line to replace */
        if (count == 1)
            fprintf(temp,"%d\n",nclients);
        else
            fprintf(temp,line);
    }

	fclose(the_file);
    fclose(temp);

    remove("dados");

    rename("replace.tmp","dados");
}

int updateNumberDesistencias()
{
    FILE *the_file;
    FILE *temp;
	the_file = fopen("dados", "r");
    temp = fopen("replace.tmp","w");
	if(the_file == NULL || temp == NULL)
	{
		perror("Unable to open the file 'dados' or 'temp.tmp'!");
		exit(1);
	}
	char line[MAXLINE];
	fgets(line, MAXLINE, the_file);
    fgets(line, MAXLINE, the_file);

	//incrementar +1
    int ndesis;
	ndesis = atoi(line) + 1;

    fseek(the_file, 0, SEEK_SET);
	int count = 0;
    while ((fgets(line, MAXLINE, the_file)) != NULL)
    {
        count++;

        /* If current line is line to replace */
        if (count == 2)
            fprintf(temp,"%d\n",ndesis);
        else
            fprintf(temp,line);
    }
    

    fclose(the_file);
    fclose(temp);

    remove("dados");

    rename("replace.tmp","dados");
}

int updatePontosTotal(int numpontos, int increm)
{
    FILE *the_file;
    FILE *temp;
	the_file = fopen("dados", "r");
    temp = fopen("replace.tmp","w");
	if(the_file == NULL || temp == NULL)
	{
		perror("Unable to open the file 'dados' or 'temp.tmp'!");
		exit(1);
	}
	char line[MAXLINE];
	fgets(line, MAXLINE, the_file);
    fgets(line, MAXLINE, the_file);
    fgets(line, MAXLINE, the_file);
	
	//incrementar +5 ou -1
    int copypontos = numpontos;
	if(increm == 5)
    {
	    copypontos = atoi(line) + 5;
    }
    else
    {
        copypontos = atoi(line) - 1;
    }

    fgets(line, MAXLINE, the_file);

    int ntent;
	ntent = atoi(line) + 1;
	
    fseek(the_file, 0, SEEK_SET);
	int count = 0;
    while ((fgets(line, MAXLINE, the_file)) != NULL)
    {
        count++;

        /* If current line is line to replace */
        if (count == 3)
            fprintf(temp,"%d\n",copypontos);
        else if (count == 4)
            fprintf(temp,"%d\n",ntent);
        else
            fprintf(temp,line);
    }
    

    fclose(the_file);
    fclose(temp);

    remove("dados");

    rename("replace.tmp","dados");


    // the_file = fopen("dados", "r+");
	// if(the_file == NULL)
	// {
	// 	perror("Unable to open the file 'dados'!");
	// 	exit(1);
	// }
	// char line1[MAXLINE];
	// fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
	// //fclose(the_file);
	// //incrementar +1
    // int ntent;
	// ntent = atoi(line1) + 1;
	// //sprintf(line, "%d", ndesis);
	// //escrever inteiro do ficheiro numclientes na primeira linha
	// // the_file = fopen("dados", "w");
	// // if(the_file == NULL)
	// // {
	// // 	perror("Unable to open the file 'dados'!");
	// // 	exit(1);
	// // }
    // fseek(the_file, 0, SEEK_SET);
    // fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
	// fflush(the_file);
    // fseek(the_file, 0, SEEK_SET);
    // fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
    // fgets(line1, MAXLINE, the_file);
	// fprintf(the_file,"%d",ntent);
	// fclose(the_file);

    return copypontos;
}
