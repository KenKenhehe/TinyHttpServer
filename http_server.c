#include "http_server.h"

void register_get(HttpServer* server,  char* uri, void (*route_function)())
{
    server->current_uri = uri;
}

RouteData init_route_data(){
    RouteData data;
    data.end_point = NULL;
    data.http_func = NULL;
    data.method = NONE;
    return data;
}


void zero_all_routes(HttpServer* server)
{
    for(int i = 0; i < ENDPOINT_LEN; ++i)
    {
        server->routes[i] = init_route_data();
    }
}

void add_route(HttpServer* server, HTTPMethod method, char* end_point_uri, char* (*http_func)())
{
    printf("adding route...\n");

    for(int i = 0; i < ENDPOINT_LEN; ++i)
    {
        if(server->routes[i].end_point == NULL)
        {
            printf("%d: Is null\n", i);
            server->routes[i].end_point = end_point_uri;
            server->routes[i].http_func = http_func;
            server->routes[i].method = method;
            break;
        }
    }

    //FOR DEBUGGING
    for (int i = 0; i < ENDPOINT_LEN; i++)
    {
        /* code */
        printf("%d, route: %d, %s\n", i, server->routes[i].method, server->routes[i].end_point);
    }
}

void handle_request(HttpServer* server, httpRequest req, httpResponse* rsp)
{
    switch (req.method)
    {
    case GET:
        for(int i = 0; i < ENDPOINT_LEN; ++i)
        {
            if(server->routes[i].method == GET && strcmp(server->routes[i].end_point, req.uri) == 0)
            {
                rsp->rsp_body = server->routes[i].http_func();
            }
        }
        break;
    case POST:
         /* code */
        for(int i = 0; i < ENDPOINT_LEN; ++i)
        {
            if(server->routes[i].method == POST && strcmp(server->routes[i].end_point, req.uri) == 0)
            {
                rsp->rsp_body = server->routes[i].http_func();
            }
        }
        break;

    default:
        break;
    }
}


int run_http_server(HttpServer* server){
    int sock_fd, new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int test = create_socket(AF_INET, SOCK_STREAM, 0, &sock_fd);
    if(test != 0) return -1;

    int bind_socket_status = bind_default_socket(&sock_fd, &address);
    if(bind_socket_status == -1) return -1;

    long val_read = 0;
    if(listen(sock_fd, 3) < 0)
    {
        printf("Fail to set up listen\n");
        return -1;
    }

    //char *response = "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    char* response = malloc(sizeof(char) * 500);

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        new_sock = accept(sock_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if(new_sock < 0)
        {
             printf("Server accept fail, status: %d, exiting.....", new_sock);
        }
       
        char buffer[MAX_BUFFER_SIZE] = {0};
        val_read = read(new_sock, buffer, MAX_BUFFER_SIZE);
        httpRequest incomming;
        httpResponse outgoing;
        outgoing.rsp_body = "";
        outgoing.status = OK_200;

        parse_http_request(&incomming, buffer);

        handle_request(server, incomming, &outgoing);
        construct_http_response(&outgoing, response);

        //printf("Got request, Method: %d, uri: %s, http version: %s\n", incomming.method, incomming.uri, incomming.http_version);

        //for each uri in endpoint
        //if this uri == uri
        //trigger the registered function for this uri
        

        write(new_sock, response, strlen(response));
        free(response);

        printf(">>>>>>>>>>>>>>>>>>> Response data sent\n");
        close(new_sock);
    }
    return 0;
}