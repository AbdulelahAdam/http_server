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


if(getaddrinfo("www.google.com", "http", &hints, &servinfo))
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

int connection_result;
if((connection_result = connect(status, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
  {
    fprintf(stderr, "connection failed!\n");
    return -1;
  }
printf("Connection succeeded with result: %d\n", connection_result);

char *msg = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n\r\n";
char* buff = (char*)malloc(4096);  
int len, bytes_sent, bytes_received;


len = strlen(msg);
bytes_sent = send(status, msg, len, 0);
while(1)
{
 bytes_received = recv(status, buff, 250, 0);
 printf("The data: %s\n", buff);
 if(bytes_received == 0)
   break;
}


// servinfo now points to a linked list of 1 or more struct addrinfos
// ... do everything until you don't need servinfo anymore ....
  //


free(buff);

freeaddrinfo(servinfo); // free the linked-list

}
