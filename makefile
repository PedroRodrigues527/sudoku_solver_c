#
#
#	Makefile for test programs
#
#


all: unix-stream-client unix-stream-server

unix-stream-client: unix-stream-client.o util.o util-stream-client.o menu.o sudoku.o
	gcc -o unix-stream-client unix-stream-client.o util.o util-stream-client.o menu.o sudoku.o

unix-stream-server: unix-stream-server.o util.o util-stream-server.o menu.o sudoku.o
	gcc -o unix-stream-server unix-stream-server.o util.o util-stream-server.o menu.o sudoku.o

clean:
	rm -f *.o *~ unix-stream-client unix-stream-server


