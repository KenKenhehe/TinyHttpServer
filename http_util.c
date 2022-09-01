#include "http_util.h"

void parse_http_start_line(struct httpRequest* request, char* line)
{
    char* token = strtok(line, " ");
    int token_idx = 0;
    
    if(strcmp((const char*)token, "GET") == 0)
    {
        request->method = GET;
    }
    else if(strcmp((const char*)token, "POST") == 0)
    {
        request->method = POST;
    }
    while (token != NULL)
    {
        printf("%d: %s\n",token_idx, token);
        switch (token_idx)
        {
        case 1:
            request->uri = token;
            break;
        case 2:
            request->http_version = token;
            break;
        default:
            break;
        }

        token = strtok(NULL, " ");
        ++token_idx;
    }
}

void parse_http_request(struct httpRequest* request, char* http_data_raw)
{
    //TODO populate the request structure with parsed data
    char* line = strtok(http_data_raw, "\r\n");
    int line_idx = 0;
    while (line != NULL)
    {
        printf("Http header start-line: %s\n", line);
        if(line_idx == 0)
        {
            parse_http_start_line(request, line);
        }
        line = strtok(NULL, "\r\n");
    }
}

char* get_status_code(StatusCode code)
{
    switch (code)
    {
    case OK_200:
        return "200 OK\n";
        break;
    case NOT_FOUND_404:
        return "404 Not Found\n";
        break;
    case NOT_IMPLEMENTED_501:
        return "501 NotImplemented\n";
        break;

    default:
        return "500 Internal Server Error\n";
        break;
    }
}

void construct_http_response(httpResponse* response, char* response_data)
{
    //Default response
    char* constructed_rsp = "";
    char* protocol_version = "HTTP/1.1 ";
    char* content_type = "Content-Type: text/plain \n";
    char* content_length_prop_str = "Content-Length: ";
    char* cors = "Access-Control-Allow-Origin: *\n";
    StatusCode rsp_status = NOT_FOUND_404; 

    int content_length = strlen(response->rsp_body);
    char content_length_str[5];
    util_itoa(content_length, content_length_str);

    char* status_code_line = get_status_code(response->status);
    //-----------------------------

    //start constructing the response 
    char* response_data_buffer = malloc(sizeof(char) * MAX_RESPONSE_LEN);

    strcpy(response_data_buffer, protocol_version); // HTTP/1.1
    strcat(response_data_buffer, status_code_line); // 200 OK
    strcat(response_data_buffer, cors);             // Access-Control...
    strcat(response_data_buffer, content_type);     // Content type: text/plain
    strcat(response_data_buffer, content_length_prop_str); // Content-Length:  
    strcat(response_data_buffer, content_length_str);// 8
    strcat(response_data_buffer, "\n\n");

    // TODO for get request, return all the item
    // TODO-LATER get item based on id
    // printf("Constructing response...\n");
    // printf("data: %s\n", response_header);
    // printf("body: %s\n", response->rsp_body);
    
    strcat(response_data_buffer, response->rsp_body);
    printf("data raw: %s", response_data_buffer);
    
    strcpy(response_data, response_data_buffer);

    free(response_data_buffer);
}

