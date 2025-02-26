#ifndef LOGGER_H
#define LOGGER_H

#include <linux/types.h>
#include <linux/fs.h>

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_TRACE 4

typedef struct {
    char *log_file;
    uint32_t log_level;
    uint32_t max_size;
    uint32_t max_files;
    bool timestamps;
    bool console_output;
} logger_config_t;

status_t logger_init(logger_config_t *config);
status_t logger_cleanup(void);

status_t logger_log(uint32_t level, const char *fmt, ...);
status_t logger_log_binary(uint32_t level, const void *data, size_t size);
status_t logger_flush(void);

status_t logger_set_level(uint32_t level);
status_t logger_rotate_logs(void);
status_t logger_set_file(const char *filename);

status_t logger_set_access_mask(uint32_t mask);
status_t logger_verify_integrity(void);

status_t logger_debug_dump(void);
status_t logger_debug_stats(void);

status_t logger_perf_start(void);
status_t logger_perf_stop(void);

status_t logger_legacy_log(const char *msg);

#endif