#include "http_server.h"

void register_get(HttpServer* server,  char* uri, void (*route_function)())
{
    server->current_uri = uri;
}

RouteData init_route_data(){
    RouteData data;
    data.end_point = "None";
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
        if(strcmp(server->routes[i].end_point, "None") == 0)
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
    //for each uri in endpoint
    //if this uri == uri

    bool match = false;
    printf("handling request...\n");
    
    //check if an endpoint exist, return 404 if not
    for(int i = 0; i < ENDPOINT_LEN; ++i)
    {
        //printf("checking: %s", (char*)server->routes[i].end_point);
        if(strcmp(server->routes[i].end_point, req.uri) != 0)
        {
            printf("%s\n", server->routes[i].end_point);
            continue;
        }
        else
        {
            match = true;
            break;
        }
    }

    if(match == false)
    {
        rsp->status = NOT_FOUND_404;
        return;
    }

    for(int i = 0; i < ENDPOINT_LEN; ++i)
    {
        if(server->routes[i].method == req.method && 
        strcmp(server->routes[i].end_point, req.uri) == 0)
        {
            rsp->rsp_body = server->routes[i].http_func();
        }
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

        
        write(new_sock, response, strlen(response));

        printf(">>>>>>>>>>>>>>>>>>> Response data sent\n");
        close(new_sock);
    }

    free(response);
    return 0;
}