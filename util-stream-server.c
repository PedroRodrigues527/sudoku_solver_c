#define MAXLINE 512

/* Servidor do tipo socket stream.
   Manda linhas recebidas de volta para o cliente */

str_echo(sockfd)
int sockfd;
{
	int n, i, inc;
	char line[MAXLINE];

	for (;;) {

		/* Le a primeira linha do socket: o incremento */
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
		printf("Cliente: %s", line);
	}
}
