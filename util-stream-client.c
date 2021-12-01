#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 512

/* Cliente do tipo socket stream.
   Le string de fp e envia para sockfd.
   Le string de sockfd e envia para stdout */

str_cli(fp, sockfd)
FILE *fp;
int sockfd;
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE+1];
	int acontecimento = 0;

	printf("PRESS ENTER\n");

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		
        /* Envia string para sockfd. Note-se que o \0 nao 
        	e enviado */
		
		n = strlen(sendline);
		if (writen(sockfd, sendline, n) != n)
			err_dump("str_cli: writen error on socket");
		
		/* Tenta ler string de sockfd. Note-se que tem de 
		   terminar a string com \0 */
		
		if(sendline[0] == '2' && strlen(sendline) == 2)
		{
			FILE *the_file = fopen("monitor", "r");
			if(the_file == NULL)
			{
				perror("Unable to open the file!");
				exit(1);
			}
			char line[MAXLINE];
			/*
			while(fgets(line, sizeof(line), the_file))
			{
				//print the line
				printf("%s", line);
				break;
			}
			*/
			while(!feof(the_file))
			{
				
    				fgets(line, MAXLINE, the_file);
			}
			
			//print the line
			printf("%s", line);
			
			fclose(the_file);

			acontecimento = 0;
		}
		
		n = readline(sockfd, recvline, MAXLINE);

		if (n<0)
			err_dump("str_cli:readline error");
		recvline[n] = 0;
		if(recvline[0] == '1' && strlen(recvline) == 3)
		{
			recvline[0] = 't';
			recvline[1] = 'u';
			acontecimento = 0;
		}

		/* Envia a string para stdout */
		fputs(recvline, stdout);
		
		int fd = open("monitor", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		//File.AppendAllText("monitor", sendline); // escreve uma linha nova o input do cliente no ficheiro de texto
		//File.AppendAllText("monitor", recvline); // escreve uma linha nova o output no ficheiro de texto
		write(fd, sendline, strlen(sendline));
		write(fd, recvline, strlen(recvline));
		//write(fd, "", 0);

		close(fd);

		switch (acontecimento)
		{
		case 0:
			printf("MAINMENU\n");
    		printf("1) Entrar no Sudoku\n");
    		printf("2) Receber log\n\n");
			break;
		default:
			break;
		}
	}
	if (ferror(fp))
		err_dump("str_cli: error reading file");
}
