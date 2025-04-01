#ifndef SERVER_H
#define SERVER_H

#include "config.h"

typedef struct {
    char method[16];
    char path[MAX_PATH_LENGTH];
    char version[16];
} HttpRequest;

void start_server(int port);
int parse_request(const char *raw_request, HttpRequest *request);

#endif