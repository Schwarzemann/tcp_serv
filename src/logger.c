#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>  // For mkdir
#include <errno.h>
#include "logger.h"

static const char *get_level_string(LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

static void ensure_log_directory() {
    struct stat st = {0};
    if (stat("logs", &st) == -1) {
        int result = mkdir("logs", 0700);
        if (result == -1 && errno != EEXIST) {
            perror("Failed to create logs directory");
        }
    }
}

static void print_to_console(const char *timestamp, LogLevel level, 
                           const char *client_ip, const char *message, 
                           const char *details) {
    printf("%s [%s] Client: %s - %s - Details: %s\n",
           timestamp, 
           get_level_string(level), 
           client_ip, 
           message, 
           details ? details : "N/A");
}

void log_message(LogLevel level, const char *client_ip, const char *message, const char *details) {
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    char timestamp[80];
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", time_info);

    // Print to console first
    print_to_console(timestamp, level, client_ip, message, details);

    // Ensure logs directory exists
    ensure_log_directory();

    FILE *log_file = fopen("logs/access.log", "a");
    if (log_file == NULL) {
        printf("Error opening log file: %s\n", strerror(errno));
        return;
    }

    fprintf(log_file, "%s [%s] Client: %s - %s - Details: %s\n",
            timestamp, 
            get_level_string(level), 
            client_ip, 
            message, 
            details ? details : "N/A");
    
    fclose(log_file);
}

void log_access(const char *client_ip, const char *method, const char *path) {
    char details[256];
    snprintf(details, sizeof(details), "Method: %s, Path: %s", method, path);
    log_message(LOG_INFO, client_ip, "Accessed server", details);
}

void log_error(const char *client_ip, const char *error_msg) {
    log_message(LOG_ERROR, client_ip, "Error occurred", error_msg);
}