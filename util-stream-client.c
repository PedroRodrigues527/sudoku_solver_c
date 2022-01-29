#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include "menu.h"

#define MAXLINE 512
pthread_mutex_t mutex;

//Funções destinada a threads
void* func1(void *valor){
	pthread_mutex_lock(&mutex);//Fecha trinco
    return (void *) 1;
	pthread_mutex_unlock(&mutex);//Abre trinco
}

void* func2(void *valor){
	pthread_mutex_lock(&mutex);
    return (void *) 2;
	pthread_mutex_unlock(&mutex);
}

void* func3(void *valor){
	pthread_mutex_lock(&mutex);
    return (void *) 0;
	pthread_mutex_unlock(&mutex);
}



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
	pthread_t id1, id2, id3; //Id das respetivas threads
	int jogador = 0; //Verifica o turno da thread (1º(0), 2º(1), 3º(2))
	void *status; //Recebe return da thread para o jogador;

	//trinco fechar

	/*
	*	Lê o ficheiro de texto e incrementa o numero de clientes (no dados.txt);
	*	dados.txt -> temp; atualiza o ficheiro temporario; renomeia temp para dados.txt;
	*/
	updateNumberClients(1);

	//trinco abrir


	printf("CARREGUE EM 'ENTER'\n\n");

	while (fgets(sendline, MAXLINE, fp) != NULL) { //Espera pelo cliente
		if(room == 2)
		{
			pthread_mutex_init(&mutex, NULL);
			//printf("antesjog: %d\n",jogador);
			if(jogador == 0)
			{

				if(pthread_create(&id1, NULL, func1, NULL) != 0)
				{
					printf("erro na criacao da tarefa 1\n");
					exit(1);
				}
				pthread_join(id1, &status);

			}
			else if(jogador == 1)
			{

				if(pthread_create(&id2, NULL, func2, NULL) != 0)
				{
					printf("erro na criacao da tarefa 2\n");
					exit(1);
				}
				pthread_join(id2, &status);

			}
			else if(jogador == 2)
			{

				if(pthread_create(&id3, NULL, func3, NULL) != 0)
				{
					printf("erro na criacao da tarefa 3\n");
					exit(1);
				}
				pthread_join(id3, &status);

			}
			jogador = (int)status;
			//printf("depoisjog: %d\n",jogador);
			//pthread_mutex_destroy(&mutex);
		}

        /* Envia string para sockfd. Note-se que o \0 nao 
        	e enviado */
		
		char fullstring[MAXLINE]; //room + texto que o cliente inseriu (room + sendline)
		sprintf(fullstring, "%d%d", room, jogador); //Sprintf: Adicionar a variavel informação

		strcat(fullstring, sendline);
		n = strlen(fullstring);
		if (writen(sockfd, fullstring, n) != n) //Verifica se o socket foi enviado com sucesso
			err_dump("str_cli: writen error on socket");
		
		/* Tenta ler string de sockfd. Note-se que tem de 
		   terminar a string com \0 */
		
		n = readline(sockfd, recvline, MAXLINE);  //Ler linha recebida pelo servidor
		if (n<0)
			err_dump("str_cli:readline error");
		recvline[n] = 0; //Ultimo caracter vazio

		/* Envia a string para stdout */	
		fputs(recvline, stdout); //Mostrar no ecrã receive line
		responseLineClient(room, linharesultado, sendline); //Responder resposta client-side
		
		printf("\n\n");
		
		room = (int)recvline[0]; //1º posição -> atualização do quarto
		jogador = (int)recvline[1]; //2º posição -> atualização do turno(do jogador em questão- qual thread a jogar)
		switch (room)
		{
		case 0:
			printf ("CARREGUE EM 'ENTER'\n\n");
			break;
		case 1:
			printf ("MENU PRINCIPAL\n1) Entrar no Sudoku\n2) Receber log\n\n");
			break;
		case 2:
			printf ("SUDOKU\nFORMATO: [Num Linha 0-8]x[Num Coluna 0-8] [Valor 1-9]\nInsire 'FF' e pressione 'Enter' para desistir do Sudoku\n");
			switch(jogador)
			{
				case 0:
					printf(" JOGADOR 1 (So linhas 0-2): \n\n");
					break;
				case 1:
					printf(" JOGADOR 2 (So linhas 3-5): \n\n");
					break;
				case 2:
					printf(" JOGADOR 3 (So linhas 6-8): \n\n");
					break;
				default:
					break;
			}
			break;
		case 8:
			//Verifica a desistência do cliente
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
				printf("DESISTIU\n");
				pthread_mutex_destroy(&mutex); //Destroi todos os trincos
                exit(1);
            }
		case 9:
			//Verifica a conclusão do sudoku
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
				printf("SUDOKU CONCLUIDO!!\n");
				pthread_mutex_destroy(&mutex);
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
