#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <stdbool.h>

struct nine_houses
{
    int data[9];
};

int find_class(int locX, int locY);
void fill(int index, int map[index][index], int y, int x);
void update(struct nine_houses class_test[], int index, int map[index][index]);
void create(struct nine_houses class_test[], int index, int map[index][index]);
void de_update(struct nine_houses class_test[], int index, int map[index][index]);
void change(struct nine_houses class_test[], int index, int first, int second);
void modify(struct nine_houses class_test[], int index, int map[index][index]);
void print(int index, int map[index][index]);
void reset_map(int index, int map[index][index]);
void reset_struct(struct nine_houses class_test[], int index);
void newBoard(int map[9][9]);
void emptyBoard(int copymap[9][9], int emptyspaces);
int verifyBoard(char *sendline,int sudokuresolver[9][9],int fullsudoku[9][9],int numlinha,int numcoluna,int valor,int points);
