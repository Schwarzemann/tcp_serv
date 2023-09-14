#include <stdio.h>
#include <time.h>
#include "logger.h"

void log_access(const char *client_ip) {
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    char timestamp[80];
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", time_info);

    FILE *log_file = fopen("logs/access.log", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    fprintf(log_file, "%s Client %s accessed the server.\n", timestamp, client_ip);
    fclose(log_file);
}
