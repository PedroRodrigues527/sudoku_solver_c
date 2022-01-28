#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "menu.h"
#define MAXLINE 512

/* Cliente do tipo socket stream.
   Le string de fp e envia para sockfd.
   Le string de sockfd e envia para stdout */

str_cli(fp, sockfd)
FILE *fp;
int sockfd;
{
	int n; //Tamanho da informação que será enviada
	char sendline[MAXLINE], recvline[MAXLINE+1], linharesultado[MAXLINE]; ////MAXLINE+1 -> devido ao \0; Mostrar opção escolhida (resposta que o cliente mandou)
	int room = 0; //Indica o menu em que o cliente esta presente

	//ficheiro de texto - adicionar +1 a num clientes no ficheiro 'dados'
	//ler inteiro do ficheiro (primeira linha - numclientes)
	//trinco fechar
	updateNumberClients(1);
	//trinco abrir


	printf("CARREGUE EM 'ENTER'\n\n");

	while (fgets(sendline, MAXLINE, fp) != NULL) { //Espera pelo cliente
		
        /* Envia string para sockfd. Note-se que o \0 nao 
        	e enviado */
		
		char fullstring[MAXLINE]; //room + texto que o cliente inseriu (room + sendline)
		sprintf(fullstring, "%d", room); //Sprintf: Adicionar a variavel informação
		strcat(fullstring, sendline);
		n = strlen(fullstring);
		if (writen(sockfd, fullstring, n) != n) //Verifica se o socket foi enviado com sucesso
			err_dump("str_cli: writen error on socket");
		
		//printf(fullstring);
		
		/* Tenta ler string de sockfd. Note-se que tem de 
		   terminar a string com \0 */
		
		n = readline(sockfd, recvline, MAXLINE);  //Ler linha recebida pelo servidor
		if (n<0)
			err_dump("str_cli:readline error");
		recvline[n] = 0; //Ultimo caracter vazi
		//printf("5");
		/* Envia a string para stdout */
		//printf("%s", recvline);
		fputs(recvline, stdout); //Mostrar no ecrã receive line
		responseLineClient(room, linharesultado, sendline); //Responder resposta client-side
		
		printf("\n\n");
		
		room = (int)recvline[0];
		switch (room)
		{
		case 0:
			printf ("CARREGUE EM 'ENTER'\n\n");
			break;
		case 1:
			printf ("MENU PRINCIPAL\n1) Entrar no Sudoku\n2) Receber log\n\n");
			break;
		case 2:
			printf ("SUDOKU\nFORMATO: [Num Linha]x[Num Coluna] [Valor]\nInsire 'FF' e pressione 'Enter' para desistir do Sudoku\n\n");
			break;
		case 8:
			//desistir o cliente
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
				printf("DESISTIU\n");
                exit(1);
            }
		case 9:
			//desistir o cliente
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
				printf("SUDOKU CONCLUIDO!!\n");
                exit(1);
            }
			break;
		default:
			break;
		}
	}
	if (ferror(fp))
		err_dump("str_cli: error reading file");
}
