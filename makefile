#
#
#	Makefile for test programs
#
#


all: unix-stream-client unix-stream-server

unix-stream-client: unix-stream-client.o util.o util-stream-client.o menu.o
	gcc -o unix-stream-client unix-stream-client.o util.o util-stream-client.o menu.o

unix-stream-server: unix-stream-server.o util.o util-stream-server.o menu.o
	gcc -o unix-stream-server unix-stream-server.o util.o util-stream-server.o menu.o

clean:
	rm -f *.o *~ unix-stream-client unix-stream-server


