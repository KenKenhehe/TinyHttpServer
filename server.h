#ifndef SERVER_H
#define SERVER_H



#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "http_util.h"

#define PORT 5000

#define MAX_BUFFER_SIZE 3000

int create_socket(int domain, int type, int protocol, int* socket_fd);
int bind_default_socket(int* socket_fd, struct sockaddr_in* address);
int run_tcp_connection(int* socket_fd, int* new_sock, struct sockaddr_in* address, int addrlen, int max_connection_num);


#endif