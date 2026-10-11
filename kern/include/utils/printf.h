/* Include guard */
#ifndef UTILS_PRINTF_H
#define UTILS_PRINTF_H

/* Includes */
#include <stdarg.h>
#include <utils/base.h>

/* Print formatted output using given putc */
extern int printf(void (*putc)(char c), const char *fmt, ...)
  __attribute__((format(printf, 2, 3)));

/* Print formatted output using given putc, given a va_list */
extern int vprintf(void (*putc)(char c), const char *fmt, va_list args);

#endif /* UTILS_PRINTF_H */
