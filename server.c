#include "server.h" 

int create_socket(int domain, int type, int protocol, int* socket_fd)
{
    *socket_fd = socket(domain, type, protocol);
    if(*socket_fd < 0)
    {
        printf("Socket creation failed! status code: %d\n", *socket_fd);
        return *socket_fd;
    }
    printf("Socket created successfully with status code: %d\n", *socket_fd);
    return 0;
}

int bind_default_socket(int* socket_fd, struct sockaddr_in* address)
{
    memset((char*)address, 0, sizeof(address));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = htonl(INADDR_ANY);
    address->sin_port = htons(PORT);
    int bind_status = 0;
    if(bind_status = bind(*socket_fd, (struct sockaddr *)address, sizeof((*address))) < 0){
        printf("Bind socket fail status code: %d, exiting...\n", bind_status);
        return -1;
    }

    printf("Socket bind successful\n");
    return 0;
}

int run_tcp_connection(int* socket_fd, int* new_sock, struct sockaddr_in* address, int addrlen, int max_connection_num)
{
    long val_read = 0;
    if(listen(*socket_fd, max_connection_num) < 0)
    {
        printf("Fail to set up listen\n");
        return -1;
    }

    char *response = "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        *new_sock = accept(*socket_fd, (struct sockaddr*)address, (socklen_t*)&addrlen);
        if(*new_sock < 0)
        {
             printf("Server accept fail, status: %d, exiting.....", *new_sock);
        }
       
        char buffer[MAX_BUFFER_SIZE] = {0};
        val_read = read(*new_sock, buffer, MAX_BUFFER_SIZE);
        httpRequest incomming;
        httpResponse outgoing;

        parse_http_request(&incomming, buffer);
        printf("Got request, Method: %d, uri: %s, http version: %s\n", incomming.method, incomming.uri, incomming.http_version);
        //construct_http_response(outgoing, &response);

        write(*new_sock, response, strlen(response));

        printf("------------------Hello message sent-------------------\n");
        close(*new_sock);
    }
    return 0;
}