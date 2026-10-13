/* Implements utils/log.h */
#include <utils/log.h>

/* Includes */
#include <utils/printf.h>

/* Log level strings */
const char *LOG_LEVELS[] = {
  [LOG_LEVEL_INFO] = "INFO: ",
  [LOG_LEVEL_WARNING] = "WARNING: ",
  [LOG_LEVEL_ERROR] = "ERROR: ",
};
const char *LOG_LEVELS_ANSI[] = {
  [LOG_LEVEL_INFO] = "\x1b[1;4;96mINFO\x1b[0m: ",
  [LOG_LEVEL_WARNING] = "\x1b[1;4;93mWARNING\x1b[0m: ",
  [LOG_LEVEL_ERROR] = "\x1b[1;4;91mERROR\x1b[0m: ",
};

/* UART */
static volatile unsigned char *uart = (unsigned char *)0x10000000;
static void uart_putc(char c) { if (c) *uart = c; }

/* Current logging targets */
static log_target_t log_target = LOG_TARGET_NONE;

/* Set logging target */
void log_target_set(log_target_t target) { log_target = target; }

/* Log a message */
void log_msg(log_level_t level, const char *format, ...) {
  void (*putc)(char) = NULL;
  bool ansi = false;
  switch (log_target) {
    case LOG_TARGET_UART: putc = uart_putc; ansi = true; break;
    case LOG_TARGET_NONE: break;
  }
  printf(putc, ansi ? LOG_LEVELS_ANSI[level] : LOG_LEVELS[level]);
  va_list args;
  va_start(args, format);
  vprintf(putc, format, args);
  va_end(args);
  printf(putc, "\r\n");
  /* TODO: kernel panic on LOG_LEVEL_ERROR */
}
