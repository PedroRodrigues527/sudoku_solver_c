#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <menu.h>
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
		
		room = menu(room, sendline);
	}
	if (ferror(fp))
		err_dump("str_cli: error reading file");
}
