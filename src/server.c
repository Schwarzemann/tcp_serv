#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "server.h"
#include "logger.h"
#include "config.h"

static void send_response(int client_sock, const char *status, const char *content_type, const char *body) {
    log_message(LOG_DEBUG, "0.0.0.0", "Entering send_response", NULL);
    char response[BUFFER_SIZE];
    int len = snprintf(response, BUFFER_SIZE,
            "HTTP/1.1 %s\r\n"
            "Server: %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            status, SERVER_NAME, content_type, strlen(body), body);
    
    if (len < 0 || len >= BUFFER_SIZE) {
        log_error("0.0.0.0", "Response buffer overflow");
        return;
    }
    
    log_message(LOG_DEBUG, "0.0.0.0", "Sending response", response);
    write(client_sock, response, strlen(response));
    log_message(LOG_DEBUG, "0.0.0.0", "Response sent", NULL);
}

int parse_request(const char *raw_request, HttpRequest *request) {
    log_message(LOG_DEBUG, "0.0.0.0", "Entering parse_request", raw_request);
    if (!raw_request || !request) {
        log_error("0.0.0.0", "Null pointer in parse_request");
        return -1;
    }
    if (sscanf(raw_request, "%15s %255s %15s", 
               request->method, request->path, request->version) != 3) {
        log_error("0.0.0.0", "Failed to parse request");
        return -1;
    }
    log_message(LOG_DEBUG, "0.0.0.0", "Request parsed successfully", NULL);
    return 0;
}

static void handle_client(int client_sock, const char *client_ip) {
    char buffer[BUFFER_SIZE];
    char debug_msg[256];
    
    log_message(LOG_DEBUG, client_ip, "Entering handle_client", NULL);
    
    int bytes_received = read(client_sock, buffer, BUFFER_SIZE - 1);
    snprintf(debug_msg, sizeof(debug_msg), "Bytes received: %d", bytes_received);
    log_message(LOG_DEBUG, client_ip, "Read request", debug_msg);
    
    if (bytes_received < 0) {
        log_error(client_ip, "Failed to read request");
        return;
    }
    buffer[bytes_received] = '\0';
    
    log_message(LOG_DEBUG, client_ip, "Request contents", buffer);

    HttpRequest request;
    if (parse_request(buffer, &request) < 0) {
        log_error(client_ip, "Invalid HTTP request format");
        send_response(client_sock, "400 Bad Request", "text/plain", "Bad Request");
        return;
    }

    log_access(client_ip, request.method, request.path);
    log_message(LOG_DEBUG, client_ip, "After log_access", NULL);

    log_message(LOG_DEBUG, client_ip, "Checking method", request.method);
    if (strcmp(request.method, "GET") == 0) {
        log_message(LOG_DEBUG, client_ip, "Method is GET", NULL);
        log_message(LOG_DEBUG, client_ip, "Checking path", request.path);
        if (strcmp(request.path, "/") == 0) {
            log_message(LOG_DEBUG, client_ip, "Path is root", NULL);
            // Simplify response to isolate issue
            const char *simple_response = "Hello, World!";
            log_message(LOG_DEBUG, client_ip, "Sending simple response", NULL);
            send_response(client_sock, "200 OK", "text/plain", simple_response);
        } else {
            log_message(LOG_DEBUG, client_ip, "Path not found", NULL);
            send_response(client_sock, "404 Not Found", "text/plain", "Page not found");
        }
    } else {
        snprintf(debug_msg, sizeof(debug_msg), "Unsupported method: %s", request.method);
        log_message(LOG_DEBUG, client_ip, "Method not implemented", debug_msg);
        send_response(client_sock, "501 Not Implemented", "text/plain", 
                     "Only GET requests are supported");
    }
    log_message(LOG_DEBUG, client_ip, "Exiting handle_client", NULL);
}

void start_server(int port) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    init_config();

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation error");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    if (listen(server_sock, 5) == -1) {
        perror("Listen error");
        exit(1);
    }

    printf("Server listening on port %d...\n", port);
    log_message(LOG_INFO, "0.0.0.0", "Server started", NULL);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == -1) {
            log_error("0.0.0.0", "Accept failed");
            continue;
        }

        char *client_ip = inet_ntoa(client_addr.sin_addr);
        log_message(LOG_DEBUG, client_ip, "Client connected", NULL);
        handle_client(client_sock, client_ip);
        close(client_sock);
    }

    close(server_sock);
}