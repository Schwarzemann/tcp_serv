#include <stdlib.h>
#include "config.h"

static char *server_root = NULL;

const char *get_server_root() {
    return server_root ? server_root : "./www";
}

void init_config() {
    // In a real implementation, this could read from a config file
    server_root = getenv("SERVER_ROOT");
}