CC=gcc

build: main.c
	$(CC) -o server.out main.c  http_server.c http_util.c server.c util.c
