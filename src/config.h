#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_PORT 80
#define BUFFER_SIZE 1024
#define MAX_PATH_LENGTH 256
#define SERVER_NAME "GrokWeb/1.0"

const char *get_server_root();
void init_config();

#endif