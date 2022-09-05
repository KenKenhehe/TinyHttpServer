#include "json_util.h"
#include "http_server.h"
void handle_get();
char* get();
char* about();

int main(){
    HttpServer server;
    zero_all_routes(&server);
    add_route(&server, GET, "/", &get);
    add_route(&server, GET, "/about", &about);
    add_route(&server, POST, "/get-item", &about);

    run_http_server(&server);
}

char* about(){
    return "About\n";
}

char* get(){
    return "Got data\n";
}

