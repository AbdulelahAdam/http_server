#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>


int initialize_server(const char* port) {
    struct addrinfo hints, *servinfo;
    int status, opt = 1;

    memset(&hints, 0, sizeof hints);  // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;      // fill in my IP for me

    if (getaddrinfo(NULL, port, &hints, &servinfo) != 0) {
        fprintf(stderr, "Failed to get address info!\n");
        return -1;
    }

    status = socket(servinfo->ai_family, servinfo->ai_socktype,
                    servinfo->ai_protocol);
    if (status == -1) {
        fprintf(stderr, "Socket failed to establish!\n");
        exit(1);
    }

    if (setsockopt(status, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(status, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        fprintf(stderr, "Port binding failed!\n");
        exit(1);
    }

    freeaddrinfo(servinfo);  // free the linked-list
    return status;           // Return the server socket status
}
