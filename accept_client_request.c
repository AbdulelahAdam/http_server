#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>



#include "./headers/parse_client_request.h"
#include "./headers/send_server_response.h"

void accept_client_request(int server_socket) {
    size_t accepted;
    size_t sock_len = sizeof(struct sockaddr); 
    char* local_addr = "localhost";
    FILE* fptr;
    char* buff_received = (char*) malloc(4096);
    char first_header[512];
    char file_contents[4096];
    char method[10], path[64], http_version[16];

    listen(server_socket, 25);
    while (1) 
    {
        accepted = accept(server_socket, (struct sockaddr*)&local_addr, (socklen_t*)&sock_len);
        if (accepted != -1) {
            printf("Accept status: %ld\n", accepted);
            printf("Connected to Server successfully.\n");
            char* file_path = (char*)malloc(512 * sizeof(char));
            // receive client request here
            // Send response here
            while (1)  // response
            {
                size_t bytes_received = recv(accepted, buff_received, 4096, 0);  
                

                if (bytes_received > 0) {
                    for (int i = 0; i < 4095 && buff_received[i] != '\n'; i++) {
                        first_header[i] = buff_received[i];
                    }
                    int value = splitHeaders(first_header, method, path, http_version); 

                    if(value == 414){
                        send_response(accepted, http_version, "414 URI Too Long", "text/html", "");
                        break;
                      }
                    else if(value == 505){
                        send_response(accepted, http_version, "505 HTTP Version Not Supported", "text/html", "");
                        break;
                      }

                    else if(value == 400){
                        send_response(accepted, http_version, "400 Bad Request", "text/html", "");
                        break;
                      }
                    
                    else
                    if (strcmp(method, "GET") == 0) {
                        sprintf(file_path, "%s%s", ".", path);
                        if (strcmp(path, "/") == 0) {
                            file_path = "./static/index.html";
                            // Add Host
                        }
                        struct stat sb;
                        if (!(stat(file_path, &sb) == 0 && S_ISDIR(sb.st_mode)))
                            fptr = fopen(file_path, "r");
                        

                        if (fptr != NULL) {
                            fseek(fptr, 0, SEEK_END);
                            size_t file_size = ftell(fptr);
                            rewind(fptr);

                            //send_response(accepted, http_version, "200 OK", "text/html", file_contents);
                              char* response_header = (char*)malloc(256);
                              sprintf(response_header, "%s %s%s%ld%s", http_version, "200 OK\r\n", "Content-Type: text/html; charset=utf-8\r\nContent-Length:", file_size, "\r\n\r\n");
                            
                              send(accepted, response_header,  strlen(response_header), 0);

                            while (fgets(file_contents, sizeof(file_contents), fptr) != NULL) {
                                if (send(accepted, file_contents, strlen(file_contents), 0) == -1) {
                                    perror("Error in sending file.");
                                    exit(1);
                                }
                                bzero(file_contents, sizeof(file_contents));
                            }
                        } else {
                            send_response(accepted, http_version,"404 File Not Found", "text/html", "");
                            break;
                        }
                        
                        fclose(fptr);
                    } else if (strcmp(method, "POST") == 0) {
                        char* message = "THIS BETTER BE WORKING!!!\n";
                        send_response(accepted, http_version, "200 OK",
                                      "text/html", message);
                        break;
                    }

                } else {
                    perror("Error receiving data. Client didn't send anything :( ");
                    send_response(accepted, http_version, "400 Bad Request", "text/html", "");
                    break;
                }
                break;
            }

            close(accepted);
        } else {
            // SERVER ERRORS SHOULD BE HERE SINCE CLIENT COULDN'T
            // CONNECT. 5XX
            send_response(accepted, http_version, "500 Internal Server Error","text/html", "");

        }
    }
    free(buff_received);
}
