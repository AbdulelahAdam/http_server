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
size_t sock_len = 25;
char* local_addr = "localhost";
char* response_header = (char*) malloc(4096);
FILE *fptr;
size_t file_size;
ssize_t bytes_received, sent_bytes;
listen(status, 2);
while(1)
{
    accepted = accept(status, (struct sockaddr* )&local_addr, (socklen_t *)&sock_len );
    if(accepted != -1)
    {
        printf("Accept status: %ld\n", accepted);
        printf("Connected to Server successfully.\n");

      // receive client request here
        //...... 
      //Send response here
      while(1) // response
      {
        fptr = fopen("./static/index.html", "r");
        bytes_received = getdelim(&buff, &file_size, '\0', fptr);
        sprintf(response_header, "%s %ld %s","HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: ", bytes_received, "\r\n\r\n");
        send(accepted, response_header, strlen(response_header), 0);

          
        if (send(accepted, buff, bytes_received, 0) == -1) {
               perror("Error in sending file.");
               exit(1);
        }

        fclose(fptr);
       //printf("%s", notes);  // Print each line read

         // bzero(buff, sizof());
          //printf("sent: %ld\n", sent_bytes);
          //bytes_received = 0;
//          if()
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
free(buff);
free(response_header);

freeaddrinfo(servinfo); // free the linked-list

}
