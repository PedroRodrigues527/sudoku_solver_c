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
	char sendline[MAXLINE];//, recvline[MAXLINE+1];
	int room = 0; //Indica menu em que o cliente esta present

	printf("CARREGUE EM 'ENTER'\n\n");

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		
        /* Envia string para sockfd. Note-se que o \0 nao 
        	e enviado */
		
		n = strlen(sendline);
		if (writen(sockfd, sendline, n) != n)
			err_dump("str_cli: writen error on socket");
		
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
			if(sendline[0] == '2' && strlen(sendline) == 2)
			{
				FILE *the_file = fopen("monitor", "r");
				if(the_file == NULL)
				{
					perror("Unable to open the file!");
					exit(1);
				}

				char line[MAXLINE];
				//Percorre as linhas do ficheiro monitor.txt
				//Cada linha é armazenada na variavel line
				//Quando chegar à ultima linha é feito uma sobreposição do line, permitindo que apenas a ultima linha seja mostrada ao utilizador
				while(!feof(the_file))
				{
					
					fgets(line, MAXLINE, the_file);
				}

				printf("Escolheu opcao 2...\n\n");
				
				//print the line
				printf("%s", line);
				
				fclose(the_file);

				room = 1;
			}
			else if(sendline[0] == '1' && strlen(sendline) == 2)
			{
				printf("Escolheu opcao 1...\n\n");
				room = 1;
			}
			else
			{
				printf("Nao escolheu nenhuma das opcoes...\n\n");
				room = 1;
			}
			break;
		
		default:
			printf("Isto nao devia acontecer...\n\n");
			break;
		}
		
		int fd = open("monitor", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		write(fd, sendline, strlen(sendline));
		close(fd);

		switch (room)
		{
		case 0:
			printf("CARREGUE EM 'ENTER'\n\n");
			break;
		case 1:
			printf("MENU PRINCIPAL\n");
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
