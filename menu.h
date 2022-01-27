#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define MAXLINE 512

int updateRoom(int room, char *sendline);
int responseLine(int room, char sendline[MAXLINE], char textuser[MAXLINE], int sudoku[9][9]);
