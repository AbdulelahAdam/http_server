#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "stdbool.h"
#include "stdio.h"

int status;
// char* key;
// char* val;
// char* delimiter;

char method[10];
char path[64];
char http_version[16];

int getaddrinfo(const char* node,     // e.g. "www.example.com" or IP
                const char* service,  // e.g. "http" or port number
                const struct addrinfo* hints, struct addrinfo** res);

void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    close(status);  // Close the server socket
    exit(0);        // Exit the program
}

// void mappifyString(char* string)
//{
// printf("new string: %s\n", string) ;
//  delimiter = strchr(string, ':');
// if (delimiter != NULL) {
// Replace '=' with a null terminator to split the string
//    *delimiter = '\0';

// key is the part before '='
//  key = string;

// value is the part after '='
// val = delimiter + 1;

// Print the key and value
// printf("%s: %s\n", key, val);
//    } else {
// printf("The string does not contain a key-value pair.\n");
//  }

// memset(&key, 0, sizeof key);
// memset(&val, 0, sizeof val);

//}

void splitHeaders(char* header) {
    // Split the string by space using strtok

    if (strlen(method) > 0)
        memset(method, '\0', sizeof(method));

    if (strlen(path) > 0)
        memset(path, '\0', sizeof(path));

    if (strlen(http_version) > 0)
        memset(http_version, '\0', sizeof(http_version));
    int token_count = 0;

    // Allocate memory for the token pointers
    char** tokens = malloc(3 * sizeof(char*));  // Adjust size as necessary
    if (tokens == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Use strtok to get tokens
    char* token = strtok(header, " ");
    while (token != NULL) {
        // Copy each token to a new string (with '\0' safely added)
        tokens[token_count] = malloc(strlen(token) + 1);  // +1 for '\0'
        if (tokens[token_count] == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }
        strcpy(tokens[token_count], token);  // Copy token to allocated space
        token_count++;
        token = strtok(NULL, " ");
    }

    if (strlen(tokens[0]) < 10)
        strncpy(method, tokens[0], strlen(tokens[0]));
    else {
        perror("HTTP Method is abnormally large\n");
        exit(1);
    }

    if (strlen(tokens[1]) < 64)
        strncpy(path, tokens[1], strlen(tokens[1]));
    else {
        perror("Parsed resource path is abnormally large\n");
        exit(1);
    }

    if (sizeof(tokens[2]) < 16)
        strncpy(http_version, tokens[2], sizeof(tokens[2]));
    else {
        perror("Parsed HTTP version is abnormally large\n");
        exit(1);
    }

    printf("Method: %s\nPath: %s\nHttp: %s\n", method, path, http_version);
    // Output the result

    // Free the allocated memory
    free(tokens);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    int opt = 1;

    struct addrinfo hints;
    struct addrinfo* servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints);  // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6

    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;      // fill in my IP for me

    if (getaddrinfo(NULL, "8080", &hints, &servinfo)) {
        fprintf(stderr, "Didn't work!!!!!!!\n");
        return -1;
    }
    status = socket(servinfo->ai_family, servinfo->ai_socktype,
                    servinfo->ai_protocol);

    if (status == -1) {
        fprintf(stderr, "socket failed to establish!\n");
        exit(1);
    }

    if (setsockopt(status, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // Make a server and listen at a port for incoming requests.

    size_t server_port;
    if ((server_port = bind(status, servinfo->ai_addr, servinfo->ai_addrlen)) ==
        -1) {
        fprintf(stderr, "port binding didn't work!\n");
        exit(1);
    }

    printf("Port binding success. Listening at port 8080.....\n");
    // listen at port for incoming requests:
    size_t accepted;
    size_t sock_len = 25;
    char* buff_received = (char*)malloc(256);
    char* buff_sent = (char*)malloc(256);
    char* local_addr = "localhost";
    char* response_header = (char*)malloc(4096 * sizeof(char));
    FILE* fptr;
    char file_contents[4096];
    char status_code[64];
    size_t file_size;
    size_t bytes_received;
    char* first_header = (char*)malloc(64);
    listen(status, 2);
    while (1) {
        accepted = accept(status, (struct sockaddr*)&local_addr,
                          (socklen_t*)&sock_len);

        if (accepted != -1) {
            printf("Accept status: %ld\n", accepted);
            printf("Connected to Server successfully.\n");
            char* file_path = (char*)malloc(512 * sizeof(char));
            // receive client request here
            // Send response here
            while (1)  // response
            {
                bytes_received = recv(accepted, buff_received, 4096, 0);
                if (bytes_received > 0) {
                    for (int i = 0; i < 128; i++) {
                        if (buff_received[i] == '\n')
                            break;

                        first_header[i] = buff_received[i];
                    }

                    splitHeaders(first_header);

                    if (strcmp(method, "GET") == 0) {
                        buff_sent = "";
                        sprintf(file_path, "%s%s", ".", path);
                        if (strlen(path) == 1) {
                            file_path = "./static/index.html";
                            // Add Host
                        }
                        fptr = fopen(file_path, "r");

                        if (fptr != NULL) {
                            fseek(fptr, 0, SEEK_END);
                            file_size = ftell(fptr);
                            rewind(fptr);
                            if (strlen(status_code) > 0)
                                memset(status_code, '\0', sizeof(status_code));

                            strncpy(status_code, "200 OK\r\n",
                                    strlen("200 OK\r\n"));

                            sprintf(response_header, "%s %s%s%ld%s",
                                    http_version, status_code,
                                    "Content-Type: text/html; "
                                    "charset=utf-8\r\nContent-Length:",
                                    file_size, "\r\n\r\n");

                            send(accepted, response_header,
                                 strlen(response_header), 0);
                            while (fgets(file_contents, sizeof(file_contents),
                                         fptr) != NULL) {
                                if (send(accepted, file_contents,
                                         strlen(file_contents), 0) == -1) {
                                    perror("Error in sending file.");
                                    exit(1);
                                }
                                bzero(file_contents, sizeof(file_contents));
                            }

                        } else {
                            // printf("status code: %s\nhttp_version: %s\nfile
                            // size: %d\n", status_code, http_version, 0);
                            memset(status_code, 0, sizeof(status_code));
                            strncpy(status_code, "404 File Not Found\r\n",
                                    strlen("404 File Not Found\r\n"));

                            sprintf(response_header, "%s %s%s%d%s",
                                    http_version, status_code,
                                    "Content-Type: text/html; "
                                    "charset=utf-8\r\nContent-Length: ",
                                    0, "\r\n\r\n");

                            send(accepted, response_header,
                                 strlen(response_header), 0);

                            break;
                        }

                        fclose(fptr);
                    } else if (strcmp(method, "POST") == 0) {
                        char* message = "THIS BETTER BE WORKING!!!\n";
                        sprintf(response_header, "%s%ld%s",
                                "HTTP/1.1 200 OK\r\nContent-Type: text/html; "
                                "charset=utf-8\r\nContent-Length:",
                                strlen(message), "\r\n\r\n");

                        send(accepted, response_header, strlen(response_header),
                             0);

                        send(accepted, message, strlen(message), 0);

                        break;
                    }

                    file_path = NULL;
                } else {
                    perror(
                        "Error receiving data. Client didn't send anything :( "
                        "\n");
                }

                break;
            }

            close(accepted);
        } else {
            // SERVER ERRORS SHOULD BE HERE SINCE CLIENT COULDN'T
            // CONNECT. 5XX
        }
    }

    printf("Server Stopped!\n");

    // free all allocated memory.
    free(local_addr);
    free(buff_sent);
    free(buff_received);
    free(response_header);
    free(first_header);
    freeaddrinfo(servinfo);  // free the linked-list
}
