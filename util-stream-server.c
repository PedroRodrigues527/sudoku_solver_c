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
		if(roomclient == 1 && textoes[0] == 49)
		{
			Main();
		}
		//VALIDACAO
		//strcpy(linharesult, "");
		responseLine(roomclient, linharesult, textoes);
		//falta validar sudoku

	}
}
