#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG
} LogLevel;

void log_message(LogLevel level, const char *client_ip, const char *message, const char *details);
void log_access(const char *client_ip, const char *method, const char *path);
void log_error(const char *client_ip, const char *error_msg);

#endif