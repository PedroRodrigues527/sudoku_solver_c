
#include <menu.h>

#define MAXLINE 512

/* Servidor do tipo socket stream.
   Manda linhas recebidas de volta para o cliente */

str_echo(sockfd)
int sockfd;
{
	int n, i, inc, roomclient;
	char line[MAXLINE];
	char lineroom[MAXLINE];
	char linharesult[MAXLINE];
	//char texto[MAXLINE];

	for (;;) {
		printf("1");
		/* Le a primeira linha do socket: os caracteres */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");
		printf("2");

		line[n-1] = '\0';
		//char *texto = line+1;

		printf("3");

		roomclient = atoi(line[0]);
		printf("4");
		//VALIDACAO
		strcpy(linharesult, "");
		responseLine(roomclient, linharesult, line);
		//falta validar sudoku
		printf("5");
		/* Manda linha de volta para o socket. n conta com
		   o \0 da string, caso contr�rio perdia-se sempre 
		   um caracter! */
		
		n = sizeof(linharesult);
		if (writen(sockfd, linharesult, n) != n)
			err_dump("str_echo: writen error");
		
		
		printf("Cliente %d no menu %d: %s\n", getpid(), roomclient, line);
	}
}
