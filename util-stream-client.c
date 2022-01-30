#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "menu.h"

#define MAXLINE 512
pthread_mutex_t mutex, mutexfile;
unsigned int state[3];

//Funcoes destinada a threads
void* func1(void *p_mystate){

	unsigned int *mystate = p_mystate;
    // XOR multiple values together to get a semi-unique seed
    *mystate = time(NULL) ^ getpid() ^ pthread_self();

	int random = rand_r(mystate) % 3; //thread safe RNG
    return (void *) random;
}

void* func2(void *p_mystate){

	unsigned int *mystate = p_mystate;
    // XOR multiple values together to get a semi-unique seed
    *mystate = time(NULL) ^ getpid() ^ pthread_self();

	int random = rand_r(mystate) % 9; //thread safe RNG
    return (void *) random;
}

/* Cliente do tipo socket stream.
   Le string de fp e envia para sockfd.
   Le string de sockfd e envia para stdout */

str_cli(fp, sockfd)
FILE *fp;
int sockfd;
{
	int n; //Tamanho da informacao que sera enviada
	char sendline[MAXLINE], recvline[MAXLINE+1], linharesultado[MAXLINE]; ////MAXLINE+1 -> devido ao \0; Mostrar opcao escolhida (resposta que o cliente mandou)
	int room = 0; //Indica o menu em que o cliente esta presente
	int jogador = 0; //Verifica o turno da thread (1º(0), 2º(1), 3º(2))
	void *status; //Recebe return da thread para o jogador;
	int arraytarefas[4];
	pthread_t thread_id[3]; //Id das respetivas threads num array

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutexfile, NULL);

	pthread_mutex_lock(&mutexfile); //trinco fechar

	/*
	*	Le o ficheiro de texto e incrementa o numero de clientes (no dados.txt);
	*	dados.txt -> temp; atualiza o ficheiro temporario; renomeia temp para dados.txt;
	*/
	updateNumberClients(1);

	pthread_mutex_unlock(&mutexfile); //trinco abrir

	srand(time(0));

	printf("CARREGUE EM 'ENTER'\n\n");

	while (fgets(sendline, MAXLINE, fp) != NULL) { //Espera pelo cliente
		if(room == 2)
		{
			pthread_mutex_lock(&mutex);//Fecha trinco
			int i;
			for (i = 0; i < 3; i++) {
				if(i == 2 || i == 1)
				{
					if (pthread_create(&thread_id[i],NULL,func2,(void *)&(state[i]))!=0) {
						printf("erro na criacao da tarefa\n");
						exit(1);
					}
				}
				else
				{
					if (pthread_create(&thread_id[i],NULL,func1,(void *)&(state[i]))!=0) {
						printf("erro na criacao da tarefa\n");
						exit(1);
					}
				}
			}
			for (i = 0; i < 3; i++)
			{
				pthread_join (thread_id[i], &status);
				arraytarefas[i] = (int)status;
			}

			if(jogador == 0)
			{
				arraytarefas[3] = 1;
			}
			else if(jogador == 1)
			{
				arraytarefas[0] += 3;
				//arraytarefas[1] += 3;
				arraytarefas[3] = 2;
			}
			else if(jogador == 2)
			{
				arraytarefas[0] += 6;
				//arraytarefas[1] += 6;
				arraytarefas[3] = 0;
			}
			jogador = arraytarefas[3];
			pthread_mutex_unlock(&mutex);//Abre trinco
		}

        /* Envia string para sockfd. Note-se que o \0 nao 
        	e enviado */
		
		char fullstring[MAXLINE]; //room + texto que o cliente inseriu (room + sendline)
		if(room == 2 && strlen(sendline) == 1)
		{
			sprintf(fullstring, "%d%d%dx%d %d", room, jogador, arraytarefas[0], arraytarefas[1], arraytarefas[2] + 1);
		}
		else
		{
			sprintf(fullstring, "%d%d", room, jogador); //Sprintf: Adicionar a variavel informacao
		}

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
		fputs(recvline, stdout); //Mostrar no ecra receive line
		responseLineClient(room, linharesultado, sendline); //Responder resposta client-side
		
		printf("\n\n");
		
		room = (int)recvline[0]; //1º posicao -> atualizacao do quarto
		jogador = (int)recvline[1]; //2º posicao -> atualizacao do turno(do jogador em questao- qual thread a jogar)
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
			//Verifica a desistencia do cliente
            if (0 == kill(getpid(), 0))
            {
                // Process exists.
				printf("DESISTIU\n");
				pthread_mutex_destroy(&mutex); //Destroi todos os trincos
                exit(1);
            }
		case 9:
			//Verifica a conclusao do sudoku
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
