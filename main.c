#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>

int status;
char* key;
char* val;
char* delimiter;


int getaddrinfo(const char *node, // e.g. "www.example.com" or IP
 const char *service, // e.g. "http" or port number
 const struct addrinfo *hints,
 struct addrinfo **res);

void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    close(status);  // Close the server socket
    exit(0);  // Exit the program
}


void mappifyString(char* string)
{
  
  delimiter = strchr(string, ':');
  if (delimiter != NULL) {
        // Replace '=' with a null terminator to split the string
        *delimiter = '\0';

        // key is the part before '='
        key = string;

        // value is the part after '='
        val = delimiter + 1;

        // Print the key and value
        printf("%s: %s\n", key, val);
    } else {
        printf("The string does not contain a key-value pair.\n");
    }

memset(&key, 0, sizeof key);
memset(&val, 0, sizeof val);

}



int main(int argc, char *argv[])
{

signal(SIGINT, handle_sigint);

int opt = 1;


struct addrinfo hints;
struct addrinfo *servinfo; // will point to the results




memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6

hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
hints.ai_flags = AI_PASSIVE; // fill in my IP for me


if(getaddrinfo(NULL, "8080", &hints, &servinfo))
  {
    fprintf(stderr, "Didn't work!!!!!!!\n");
    return -1;
  }
status = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

if(status == -1)
  {
    fprintf(stderr, "socket failed to establish!\n");
    exit(1);
  }

if (setsockopt(status, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
}
//Make a server and listen at a port for incoming requests.

size_t server_port;
if((server_port = bind(status, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
{
 fprintf(stderr, "port binding didn't work!\n");
 exit(1);
}

printf("Port binding success. Listening at port 8080.....\n");
// listen at port for incoming requests:
size_t accepted;
size_t sock_len = 25;
char* buff_received = (char*)malloc(256 * sizeof(char));
char* buff_sent = (char*)malloc(4096 * sizeof(char));
char* local_addr = "localhost";
char* response_header = (char*) malloc(256 * sizeof(char));
FILE* fptr;
size_t file_size;
size_t bytes_received;

listen(status, 2);
while(1)
{
    accepted = accept(status, (struct sockaddr* )&local_addr, (socklen_t *)&sock_len );
    if(accepted != -1)
    {
        printf("Accept status: %ld\n", accepted);
        printf("Connected to Server successfully.\n");

      // receive client request here
      //Send response here
      while(1) // response
      { 
        bytes_received = recv(accepted, buff_received, 4096, 0);
        if(bytes_received > 0)
        {
          //method[7] = '\0';
   
          mappifyString(buff_received);

          buff_sent = "<h1 style='text-align: center; color: red;'>This page is under construction</h1>\n";
          file_size = strlen(buff_sent);
          sprintf(response_header, "%s %ld %s","HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: ", file_size, "\r\n\r\n");
        //printf("%s\n", method);
        
        send(accepted,response_header, strlen(response_header), 0);
          
        if (send(accepted, buff_sent, file_size, 0) == -1) 
          {
               perror("Error in sending file.");
               exit(1);
          }
          memset(buff_received, 0, sizeof buff_received);

        }
        else{
          perror("Error receiving data. Client didn't send anything :( \n");
        }

         break;
        

      }

      close(accepted);
      
    }
}

printf("Server Stopped!\n");
//close(status);



/* Act as client. Send request and receive reponse from server.
int connection_result;
if((connection_result = connect(status, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
  {
    fprintf(stderr, "connection failed!\n");
    return -1;
  }
printf("Connection succeeded with result: %d\n", connection_result);

char *msg = "GET /posts/1 HTTP/1.1\r\nHost: jsonplaceholder.typicode.com\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n\r\n";
char* buff = (char*)malloc(155);  
int len, bytes_sent, bytes_received;


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
fclose(fptr);
free(local_addr);
free(buff_sent);
free(buff_received);
free(response_header);
free(key);
free(val);
free(delimiter);
freeaddrinfo(servinfo); // free the linked-list

}
