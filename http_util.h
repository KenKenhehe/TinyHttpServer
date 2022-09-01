#ifndef HTTP_H
#define HTTP_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"


#define MAX_RESPONSE_LEN 500

typedef enum HTTPMethod{
    NONE = 0,
    GET = 1,
    POST = 2,
    PUT = 3,
    DELETE = 4
} HTTPMethod;

typedef enum StatusCode
{
    OK_200,             //200
    NOT_FOUND_404,      //404
    NOT_IMPLEMENTED_501 //501
} StatusCode;

typedef struct httpRequest
{
    enum HTTPMethod method;
    char* uri;
    char* http_version;
} httpRequest;

typedef struct httpResponse
{
    char* http_version;
    StatusCode status;
    int content_len;
    char* content_type;
    char* rsp_body;
} httpResponse;


void parse_http_request(struct httpRequest* request, char* http_data_raw);
void parse_http_start_line(struct httpRequest* request, char* http_data_raw);

void construct_http_response(httpResponse* response, char* response_data);

char* get_status_code(StatusCode code);

void handle_get(char* uri);
void handle_post(char** response_body, char* uri);
void handle_put(char** response_body, char* uri);
void handle_delete(char** response_body, char* uri);



#endif