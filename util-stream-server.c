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
	/*
	* 	n: Tamanho do readline (que recebe do cliente)
	*	roomclient: Atualiza room, depois envia para o cliente; 
	*	turnojogador: turno do jogador(thread) para enviar para o cliente;
	*/
	int n,roomclient, turnojogador;
	char line[MAXLINE]; //Linha socket recebida pelo cliente;
	char linharesult[MAXLINE]; //Incremento da resposta pelo servidor;
	char textoes[MAXLINE]; //Input do cliente, sem o room e o jogador;
	int sudoku[9][9]; //Matriz de jogo final;

	newBoard(sudoku); //Cria sudoku final
	int sudokuresolver[9][9]; //Vai ser resolvido pelo servidor (mostrado ao cliente pelo servidor)
	int points = 0;

	/*
	*	1.Preparar sudoku para ser apresentado ao cliente;
	* 	2.Preenchimento do sudokuresolver completo;
	*	3.Colocar espaços vazios até 20 posições no máximo;
	*/
	int u, v;
    for (u = 0; u < 9; u++)
        for (v = 0; v < 9; v++)
            sudokuresolver[u][v] = sudoku[u][v]; //1. 2.
	
	emptyBoard(sudokuresolver, 20); //3.

	for (;;) {

		/* Le a primeira linha do socket: os caracteres */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");

		char *texto = line+2; //Incrementar apontador ocultando as 2º posições (o room e o turno do jogador);
		strcpy(textoes, texto); // textoes = texto (variaveis)

		//Conversão de ASCII
		roomclient = (int)line[0] - 48; 
		turnojogador = (int)line[1] - 48;

		/*
		*	FORMATO: Cliente getpid()-1 no menu roomclient: textoes
		*/

		char clienteservidor[MAXLINE]; //Cliente, servidor, resposta do server, resposta da opção
		sprintf(clienteservidor,""); //clienteservidor = "";
		strcat(clienteservidor, "Cliente "); //clienteservidor += "Cliente"

		char idcliente[MAXLINE]; //temp para guardar informação
		sprintf(idcliente, "%d", getpid()-1);
		strcat(clienteservidor, idcliente); //clienteservidor += idcliente

		strcat(clienteservidor, " no menu "); //clienteservidor += " no menu "

		sprintf(idcliente, "%d", roomclient);
		strcat(clienteservidor, idcliente);  //clienteservidor += roomclient

		strcat(clienteservidor, ": ");  //clienteservidor += ": ";

		sprintf(idcliente, "%s", textoes);
		strcat(clienteservidor, idcliente); //clienteservidor += textoes

		//RESPOSTA DO SERVIDOR
		points = responseLine(roomclient, linharesult, textoes, sudokuresolver, sudoku, points, turnojogador);

		strcat(clienteservidor, linharesult); //clienteservidor += linharesult(resosta conforme a opção)

		//verifycomplete
		int isover; //Verifica a conclusão do sudoku
		isover = isFinished(sudokuresolver, sudoku); //1: acabou; 0: caso contrario
		if(isover == 1)
		{
			//Incrementa o clienteservidor com a string correspondente
			strcat(clienteservidor,"SUDOKU CONCLUIDO!!\n");
			sprintf(idcliente, "PONTOS: %d\n", points);
			strcat(clienteservidor, idcliente);

			//trinco fechar
			updateNumberClients(0); //Atualiza os ficheiro dados.txt
			//trinco abrir

			roomclient = 9;
		}
		else
		{
			roomclient = updateRoom(roomclient, textoes); //Atualiza o room
		}

		printf(clienteservidor);

		//Fechar trinco
		int fd = open("servidor", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		write(fd, clienteservidor, strlen(clienteservidor));
		close(fd);
		//Abrir trinco

		line[0] = roomclient;
		line[1] = turnojogador;

		/* Manda linha de volta para o socket. n conta com
		   o \0 da string, caso contr�rio perdia-se sempre 
		   um caracter! */
		if (writen(sockfd, line, n) != n)
			err_dump("str_echo: writen error");

	}
}
