#include "sudoku.h"
#include <menu.h>

#define MAXLINE 512

/* Servidor do tipo socket stream.
   Manda linhas recebidas de volta para o cliente */

str_echo(sockfd)
int sockfd;
{
	int n, i, roomclient;
	char line[MAXLINE];
	char linharesult[MAXLINE];
	char textoes[MAXLINE];
	int sudoku[9][9];
	newBoard(sudoku);
	int sudokuresolver[9][9];
	//int sudoku[9][9] = newBoard();
	int u, v;
    for (u = 0; u < 9; u++)
        for (v = 0; v < 9; v++)
            sudokuresolver[u][v] = sudoku[u][v];
	
	//emptyBoard(sudokuresolver, 20);
	//sudoku = newBoard();
	//int resolversudoku[9][9];
	//memcpy(resolversudoku, emptyBoard(sudoku[9][9]), sizeof resolversudoku);
	//resolversudoku = emptyBoard(sudoku[9][9]);
	//int numberplayers[3];

	for (;;) {
		/* Le a primeira linha do socket: os caracteres */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");

		/* Manda linha de volta para o socket. n conta com
		   o \0 da string, caso contr�rio perdia-se sempre 
		   um caracter! */
		if (writen(sockfd, line, n) != n)
			err_dump("str_echo: writen error");

		char *texto = line+1;
		strcpy(textoes, texto);

		roomclient = (int)line[0] - 48;

		printf("Cliente %d no menu %d: %s", getpid(), roomclient, textoes);
		/*
		if(roomclient == 1 && textoes[0] == 49 )//&& PIDinArray(numberplayers, getpid()) == 0 && isArrayEmpty() == 1)
		{
			int sudoku[9][9] = newBoard();
		}
		*/
		//int sudoku[9][9] = newBoard();
		//VALIDACAO
		//numberplayers = responseLine(roomclient, linharesult, textoes, numberplayers);
		responseLine(roomclient, linharesult, textoes, sudokuresolver);
		//validarsudokunestalinha
		//print(9, sudoku);
		/*
		if(roomclient == 1 && textoes[0] == 49)
		{
			if(numberplayers >= 0 && numberplayers < 3)
			{
				numberplayers++;
			}
			else if(numberplayers >= 3)
			{
				printf("Infelizmente, nao pode entrar porque já tem numero de jogadores maximo (3)...\n");
			}
		}
		*/

	}
}
