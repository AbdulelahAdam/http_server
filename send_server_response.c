#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void send_response(int client_socket, const char* http_version, const char* status_code, const char* content_type, const char* body) 
{
    char response_header[512];
   
    sprintf(response_header,"%s %s\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", http_version, status_code, content_type, strlen(body));

    send(client_socket, response_header, strlen(response_header), 0);
    send(client_socket, body, strlen(body), 0);
  
}
