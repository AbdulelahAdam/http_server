#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "stdbool.h"
#include <sys/types.h>

#include "stdio.h"

int status;
// char* key;
// char* val;
// char* delimiter;

char* method;
char* path;
char* http_version;

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
    char* token = strtok(header, " ");
    int i = -1;
    while (token != NULL) {
        // Store the token
        i++;

        if (i == 0)
            method = token;

        else if (i == 1)
            path = token;

        else {
            http_version = token;
            http_version[strlen(http_version) - 1] = '\0';
        }

        // Get the next token
        token = strtok(NULL, " ");
    }

    // Output the result

    // Free the allocated memory
    free(token);
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
    char* status_code = (char*)malloc(16);
    size_t file_size;
    size_t bytes_received;
    char* first_header = (char*)malloc(64);
    
    listen(status, 2);
    while (1) {
        accepted = accept(status, (struct sockaddr*)&local_addr,
                          (socklen_t*)&sock_len);
        status_code = "200 OK\r\n";
        if (accepted != -1) {
            printf("Accept status: %ld\n", accepted);
            printf("Connected to Server successfully.\n");

            // receive client request here
            // Send response here
            while (1)  // response
            {
                bytes_received = recv(accepted, buff_received, 4096, 0);
                if (bytes_received > 0)
                {
                    for (int i = 0; i < 128; i++) {
                        if (buff_received[i] == '\n')
                            break;

                        first_header[i] = buff_received[i];
                    }

                    splitHeaders(first_header);
                    //printf("method: %s", method);
                    if(strcmp(method,"GET") == 0)
                    {
                    buff_sent = "";
                    char* file_path = (char*)malloc(32);
                    sprintf(file_path, "%s%s", ".", path);
                    if (strlen(path) == 1) {
                        file_path = "./static/index.html";
                        // Add Host
                    }

                    fptr = fopen(file_path, "r"); 


                    if (fptr != NULL) {
                        // buff_sent = fptr;
                        fseek(fptr, 0, SEEK_END);
                        file_size = ftell(fptr);
                        rewind(fptr);
                        //    file_size = 0;
                        sprintf(response_header, "%s %s%s %ld %s", http_version,
                                status_code,
                                "Content-Type: text/html; "
                                "charset=utf-8\r\nContent-Length: ",
                                file_size, "\r\n\r\n");
                        //        printf("%s\n", first_header);
                        send(accepted, response_header, strlen(response_header),
                             0);
                        while (fgets(file_contents, sizeof(file_contents),
                                     fptr) != NULL) {
                            if (send(accepted, file_contents,
                                     strlen(file_contents), 0) == -1) {
                                perror("Error in sending file.");
                                exit(1);
                            }
                            bzero(file_contents, sizeof(file_contents));
                        }
                        //memset(http_version, 0, sizeof http_version);

                    } else {
                        // printf("status code: %s\nhttp_version: %s\nfile size:
                        // %d\n", status_code, http_version, 0);
                        sprintf(response_header, "%s %s%d%s", http_version,
                                "404 File Not Found\r\nContent-Type: "
                                "text/html; charset=utf-8\r\nContent-Length: ",
                                0, "\r\n\r\n");
                        send(accepted, response_header, strlen(response_header),
                             0);
                        memset(http_version, 0, sizeof http_version);

                        break;

                    }
                  }
                  else if(strcmp(method,"POST") == 0)
                   {

                      char* message = "THIS BETTER BE WORKING!!!\n";
                      sprintf(response_header, "%s %ld %s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: ",
                                strlen(message), "\r\n\r\n");

                        send(accepted, response_header, strlen(response_header), 0);

                        send(accepted, message, strlen(message), 0);
                      
                        break;
                      
                   } 
                    // memset(buff_received, 0, sizeof buff_received);
                    // memset(file_path, 0, sizeof file_path);
                } else {
                    
                    perror(
                        "Error receiving data. Client didn't send anything :( "
                        "\n");
                }

                 //memset(response_header, 0, sizeof response_header);
                memset(http_version, 0, sizeof http_version);
                // memset(path, 0, sizeof path);
                // memset(method, 0, sizeof method);

                fclose(fptr);
                break;
            }

            close(accepted);
        }
          else {
            // SERVER ERRORS SHOULD BE HERE SINCE CLIENT COULDN'T
                    // CONNECT. 5XX
          }
    }

    printf("Server Stopped!\n");
    // close(status);

    /* Act as client. Send request and receive reponse from server.
    int connection_result;
    if((connection_result = connect(status, servinfo->ai_addr,
    servinfo->ai_addrlen)) == -1)
      {
        fprintf(stderr, "connection failed!\n");
        return -1;
      }
    printf("Connection succeeded with result: %d\n", connection_result);

    char *msg = "GET /posts/1 HTTP/1.1\r\nHost:
    jsonplaceholder.typicode.com\r\nContent-Type:
    application/json\r\nConnection: close\r\n\r\n\r\n"; char* buff =
    (char*)malloc(155); int len, bytes_sent, bytes_received;


    len = strlen(msg);
    bytes_sent = send(status, msg, len, 0);
    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen("./result.json", "a");

    while(1)
    {
     bytes_received = recv(status, buff, 155, 0);
     fprintf(fptr, buff);
    // printf("%s\n", buff);
     if(bytes_received == 0)
       break;
    }



    // free all allocated memory.
    */
    free(local_addr);
    free(buff_sent);
    free(buff_received);
    free(response_header);
    free(method);
    //free(file_path);
    free(first_header);
    free(path);
    free(http_version);
    free(status_code);
    freeaddrinfo(servinfo);  // free the linked-list
}
