
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
	//char texto[MAXLINE];

	for (;;) {
		/* Le a primeira linha do socket: os caracteres */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");

		line[n-1] = '\0';
		char *texto = line+1;

		roomclient = (int)line[0] - 48;

		//VALIDACAO
		//strcpy(linharesult, "");
		responseLine(roomclient, linharesult, texto);
		//falta validar sudoku

		/* Manda linha de volta para o socket. n conta com
		   o \0 da string, caso contr�rio perdia-se sempre 
		   um caracter! */
		n = sizeof(linharesult);
		printf("%d",n);
		if (writen(sockfd, linharesult, n) != n)
			err_dump("str_echo: writen error");
		
		
		printf("Cliente %d no menu %d: %s\n", getpid(), roomclient, texto);
	}
}
