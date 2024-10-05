#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int getaddrinfo(const char *node, // e.g. "www.example.com" or IP
 const char *service, // e.g. "http" or port number
 const struct addrinfo *hints,
 struct addrinfo **res);




int main(int argc, char *argv[])
{

int status;
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
char* buff = (char*)malloc(4096);  
size_t sock_len = 55;
char* local_addr = "localhost";
FILE *fptr;
char notes[4096];
ssize_t bytes_received, sent_bytes;
listen(status, 255);
while(1)
{
    accepted = accept(status, (struct sockaddr* )&local_addr, (socklen_t *)&sock_len );
    if(accepted != -1)
    {
        printf("Accpet status: %ld\n", accepted);
        printf("Connected to Server successfully.\n");
        bytes_received = recv(accepted, buff, 4096, 0);

        if(bytes_received != 0)
        {
          printf("%c\n", (char)(buff[4]));
          fptr = fopen("./notes.txt", "r");
          fgets(notes, 4096, fptr);
          sent_bytes = send(accepted, notes , sizeof(notes), 0);
          printf("sent: %ld\n", sent_bytes);
//          if()
        }
    }
}

printf("Server Stopped!\n");
close(accepted);
close(status);




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
free(buff);

freeaddrinfo(servinfo); // free the linked-list

}
