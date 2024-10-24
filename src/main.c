#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/initialize_server.h"
#include "../include/accept_client_request.h"

void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    // Initialize the server and bind to port 8080
    int server_socket = initialize_server("8080");
    if (server_socket == -1) {
        fprintf(stderr, "Failed to initialize the server.\n");
        exit(1);
    }

    printf("Server initialized. Listening on port 8080...\n");

    // Accept client requests
    accept_client_request(server_socket);

    // Cleanup (optional as program will terminate)
    close(server_socket);
    return 0;
}
