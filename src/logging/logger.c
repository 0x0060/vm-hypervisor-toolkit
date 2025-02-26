#include "logging/logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

static logger_state_t *state = NULL;
#define MAX_BUFFER_SIZE (1024 * 1024)

static char log_buffer[MAX_BUFFER_SIZE];
static size_t buffer_pos = 0;
static FILE *log_file = NULL;
static uint32_t current_log_level = LOG_LEVEL_INFO;
static char log_filename[256];
static uint32_t max_file_size = 10 * 1024 * 1024;
static uint32_t max_backup_count = 5;

int logger_init(const char *filename, uint32_t level) {
    if (!filename) return -1;
    if (state != NULL) return -1;

    state = malloc(sizeof(logger_state_t));
    if (!state) return -1;

    memset(state, 0, sizeof(logger_state_t));
    strncpy(log_filename, filename, sizeof(log_filename) - 1);
    current_log_level = level;

    log_file = fopen(filename, "a");
    if (!log_file) {
        free(state);
        state = NULL;
        return -1;
    }

    return 0;
}

int logger_cleanup(void) {
    if (!state) return -1;

    if (buffer_pos > 0) {
        logger_flush_buffer();
    }

    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }

    free(state);
    state = NULL;
    return 0;
}

static void logger_write(const char *component, uint32_t level, const char *fmt, va_list args) {
    char timestamp[32];
    time_t now;
    struct tm *tm_info;

    if (!state || !log_file) return;
    if (level < current_log_level) return;

    time(&now);
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    int written = snprintf(log_buffer + buffer_pos, MAX_BUFFER_SIZE - buffer_pos,
                          "[%s][%s] ", timestamp, component);
    if (written > 0) buffer_pos += written;

    written = vsnprintf(log_buffer + buffer_pos, MAX_BUFFER_SIZE - buffer_pos, fmt, args);
    if (written > 0) buffer_pos += written;

    log_buffer[buffer_pos++] = '\n';

    if (buffer_pos >= MAX_BUFFER_SIZE - 1024) {
        logger_flush_buffer();
    }
}

void log_error(const char *component, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write(component, LOG_LEVEL_ERROR, fmt, args);
    logger_flush_buffer();
    va_end(args);

void log_warning(const char *component, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write(component, LOG_LEVEL_WARNING, fmt, args);
    va_end(args);
}

void log_info(const char *component, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write(component, LOG_LEVEL_INFO, fmt, args);
    va_end(args);
}

void log_debug(const char *component, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write(component, LOG_LEVEL_DEBUG, fmt, args);
    va_end(args);
}

int logger_rotate_files(void) {
    char old_name[270], new_name[270];
    struct stat st;

    if (!state || !log_file) return -1;
    if (stat(log_filename, &st) == -1) return -1;
    if (st.st_size < max_file_size) return 0;

    fclose(log_file);

    for (int i = max_backup_count - 1; i > 0; i--) {
        snprintf(old_name, sizeof(old_name), "%s.%d", log_filename, i);
        snprintf(new_name, sizeof(new_name), "%s.%d", log_filename, i + 1);
        rename(old_name, new_name);
    }

    snprintf(new_name, sizeof(new_name), "%s.1", log_filename);
    rename(log_filename, new_name);

    log_file = fopen(log_filename, "a");
    if (!log_file) {
        return -1;
    }

    return 0;
}

int logger_set_level(uint32_t level) {
    if (!state) return -1;
    current_log_level = level;
    return 0;
}

void logger_flush_buffer(void) {
    if (buffer_pos > 0 && log_file) {
        fwrite(log_buffer, 1, buffer_pos, log_file);
        fflush(log_file);
        buffer_pos = 0;
    }
}

int logger_set_max_file_size(uint32_t size) {
    if (!state || size == 0) return -1;
    max_file_size = size;
    return 0;
}

int logger_set_backup_count(uint32_t count) {
    if (!state || count == 0) return -1;
    max_backup_count = count;
    return 0;
}

void log_violation(const char *message) {
    if (!state) return;
    log_error("SECURITY", "%s", message);
}

void log_performance(const char *component, uint64_t duration_ns) {
    if (!state) return;
    log_debug(component, "Operation took %lu ns", duration_ns);
}

void log_legacy_message(const char *message) {
    if (!state) return;
    log_warning("LEGACY", "%s", message);
}
}