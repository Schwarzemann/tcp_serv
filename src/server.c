#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "server.h"
#include "logger.h"

void start_server(int port) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) == -1) {
        perror("Listen error");
        exit(1);
    }

    printf("Server listening on port %d...\n", port);

    // Accept and handle client connections
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == -1) {
            perror("Accept error");
            continue;
        }

        log_access(inet_ntoa(client_addr.sin_addr)); // Log the client's IP address

        // Handle client request (you can add your HTTP request handling logic here)
        // For a complete web server, you'd implement HTTP parsing and response handling.

        close(client_sock); // Close the client socket
    }

    close(server_sock); // Close the server socket (this will never be reached)
}
