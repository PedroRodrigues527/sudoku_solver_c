#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "sudoku.h"
#include "menu.h"

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
	int points = 0;
	int u, v;
    for (u = 0; u < 9; u++)
        for (v = 0; v < 9; v++)
            sudokuresolver[u][v] = sudoku[u][v];
	
	emptyBoard(sudokuresolver, 20);

	for (;;) {
		/* Le a primeira linha do socket: os caracteres */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");

		char *texto = line+1;
		strcpy(textoes, texto);

		roomclient = (int)line[0] - 48;

		char clienteservidor[MAXLINE];
		sprintf(clienteservidor,"");
		strcat(clienteservidor, "Cliente ");

		char idcliente[MAXLINE];
		sprintf(idcliente, "%d", getpid()-1);
		strcat(clienteservidor, idcliente);

		strcat(clienteservidor, " no menu ");

		sprintf(idcliente, "%d", roomclient);
		strcat(clienteservidor, idcliente);

		strcat(clienteservidor, ": ");

		sprintf(idcliente, "%s", textoes);
		strcat(clienteservidor, idcliente);

		//RESPOSTA DO SERVIDOR
		points = responseLine(roomclient, linharesult, textoes, sudokuresolver, sudoku, points);

		strcat(clienteservidor, linharesult);

		//verifycomplete
		int isover;
		isover = isFinished(sudokuresolver, sudoku);
		if(isover == 1)
		{
			strcat(clienteservidor,"SUDOKU CONCLUIDO!!\n");
			//trinco fechar
			updateNumberClients(0);
			//trinco abrir
			roomclient = 9;
		}
		else
		{
			roomclient = updateRoom(roomclient, textoes);
		}

		printf(clienteservidor);

		//trincofecha
		int fd = open("servidor", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		write(fd, clienteservidor, strlen(clienteservidor));
		close(fd);
		//trincoabre

		line[0] = roomclient;

		/* Manda linha de volta para o socket. n conta com
		   o \0 da string, caso contr�rio perdia-se sempre 
		   um caracter! */
		if (writen(sockfd, line, n) != n)
			err_dump("str_echo: writen error");
		
		
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
