#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "http_util.h"
#include "server.h"
#include "stdbool.h"

#define ENDPOINT_LEN 10
#define MAX_ENDPOINT_LEN 100

typedef struct RouteData
{
    char* end_point;
    char* (*http_func)();
    HTTPMethod method;
} RouteData;

//needs a dict for <endpoint, method, function>
typedef struct HttpServer
{
    int* tcp_sock_fd; //fd that describe the tcp connection
    StatusCode current_status_code;
    char* current_uri;
    HTTPMethod retrived_method;
    RouteData routes[ENDPOINT_LEN];
} HttpServer;

void handle_request(HttpServer* server, httpRequest req, httpResponse* rsp);

void register_get(HttpServer* server,  char* uri, void (*route_function)());
void register_post(HttpServer* server,  char* uri, void (*route_function)());

int run_http_server(HttpServer* server);

void zero_all_routes(HttpServer* server);
RouteData init_route_data();
void add_route(HttpServer* server, HTTPMethod method, char* end_point_uri, char* (*http_func)());

#endif