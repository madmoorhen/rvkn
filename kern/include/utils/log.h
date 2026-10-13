/* Include guard */
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

/* Includes */
#include <utils/base.h>

/* Log levels */
typedef enum {
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
} log_level_t;

/* Log targets */
typedef enum {
  LOG_TARGET_NONE,
  LOG_TARGET_UART,
} log_target_t;

/* Set logging target */
extern void log_target_set(log_target_t target);

/* Log a message */
extern void log_msg(log_level_t level, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

#endif /* UTILS_LOG_H */
