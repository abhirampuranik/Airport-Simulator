a.exe:client.o server.o
	gcc client.o server.o -lm
client.o:client.c server.c
	gcc -c client.c -lm
server.o:server.c server.h
	gcc -c server.c -lm