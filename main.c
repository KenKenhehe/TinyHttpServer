#include "json_util.h"
#include "http_server.h"
void handle_get();
char* get();
char* about();

int main(){
    // int sock_fd, new_sock;
    // struct sockaddr_in address;
    // int addrlen = sizeof(address);

    // int test = create_socket(AF_INET, SOCK_STREAM, 0, &sock_fd);
    // if(test != 0) return -1;

    // int bind_socket_status = bind_default_socket(&sock_fd, &address);
    // if(bind_socket_status == -1) return -1;

    // //register_get("/", home)

    // int server_status = run_server(&sock_fd, &new_sock, &address, addrlen, 3);
    // return 0;

    HttpServer server;
    // register_get(&server, "", get);
    zero_all_routes(&server);
    add_route(&server, GET, "/", &get);
    add_route(&server, GET, "/about", &about);
    add_route(&server, POST, "/sdw", &about);
    add_route(&server, GET, "/wwwt", &about);

    run_http_server(&server);
}


char* about(){
    return "About\n";
}

char* get(){
    printf("Got GET request with heroes\n");
    return "Got data\n";
}

