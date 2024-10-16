#ifndef SEND_SERVER_RESPONSE_H
#define SEND_SERVER_RESPONSE_H

void send_response(int client_socket, const char* http_version, const char* status_code, const char* content_type, const char* body);

#endif
