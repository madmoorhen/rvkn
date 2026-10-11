/* Implements utils/printf.h */
#include <utils/printf.h>

/* Includes */
#include <stdarg.h>

/* A putc that does nothing */
static void putc_ignore(char c) { }

/* Print formatted output using given putc */
int printf(void (*putc)(char c), const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  /* Good for getting size of output, allocating buf, then actually printing */
  if (!putc) putc = putc_ignore;

  u64_t cur = 0;
  int chars_out = 0;
  while (1) {
    if (fmt[cur] == '\0') break;
    if (fmt[cur] != '%') {
      putc(fmt[cur]); chars_out++;
      cur++;
      continue;
    }
    cur++;
    bool right_align = true;
    char positive_sign = 0;
    bool zero_pad = false;
    bool thousands_comma = false; /* NOTE: not implemented here */
    bool always_decimal = false;
    bool base_prefix = false; /* Add 0x/0X/0o/0O */
    /* It seems 0 is standard rather than 0o but that could get confusing */

    /* Parse flags */
  flags:
    switch (fmt[cur]) {
      case '-': right_align = false; cur++; goto flags;
      case '+': positive_sign = '+'; cur++; goto flags;
      case ' ': positive_sign = ' '; cur++; goto flags;
      case '0': zero_pad = true; cur++; goto flags;
      case '\'': thousands_comma = true; cur++; goto flags;
      case '#': always_decimal = true; base_prefix = true; cur++; goto flags;
    }

    /* Parse (minimum) width */
    u64_t width = 0;
    if (fmt[cur] == '*') {
      width = (u64_t)va_arg(args, unsigned long long); cur++;
    }
    while (fmt[cur] >= '0' && fmt[cur] <= '9') {
      width *= 10;
      width += fmt[cur] - '0';
      cur++;
    }

    /* Parse precision */
    u64_t precision = UINT64_MAX;
    if (fmt[cur] == '.') {
      cur++;
      if (fmt[cur] == '*') {
        precision = (u64_t)va_arg(args, unsigned long long); cur++;
      } else {
        precision = 0;
        while (fmt[cur] >= '0' && fmt[cur] <= '9') {
          precision *= 10;
          precision += fmt[cur] - '0';
          cur++;
        }
      }
    }

    /* Parse length */
    u64_t length = 4;
    if (fmt[cur] == 'l') { cur++; if (fmt[cur] == 'l') { cur++; length = 8; } }

    /* Parse type */
    bool capital = false;
    switch (fmt[cur]) {

      case '%': /* Just % */
        cur++;
        putc('%'); chars_out++;
        break;

      case 'c': /* Char */
        cur++;
        putc(va_arg(args, int)&0xff); chars_out++;
        break;

      case 's': { /* String */
        cur++;
        const char *s = va_arg(args, const char *);
        u64_t len = MIN(strlen(s), precision);
        if (right_align && len < width) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(' '); chars_out++;
          }
        }
        for (u64_t i = 0; i < len; i++) { putc(s[i]); chars_out++; }
        if (!right_align && len < width) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(' '); chars_out++;
          }
        }
      } break;

      case 'u': { /* Unsigned decimal integer */
        cur++;

        u64_t val = (u64_t)va_arg(args, unsigned long long);
        /*if (length == 8) val = va_arg(args, unsigned long long);
        else val = va_arg(args, unsigned int);*/

        u64_t digits = 0;
        u64_t n = val;
        while (n) { n /= 10; digits++; }
        if (digits == 0) digits = 1;

        if (right_align && digits < width) {
          for(u64_t i = 0; i < width - digits; i++) {
            putc(zero_pad ? '0' : ' '); chars_out++;
          }
        }
        for (u64_t i = 0; i < digits; i++) {
          u64_t v = val;
          for (u64_t j = 0; j < digits - i - 1; j++) v /= 10;
          putc('0' + (v % 10)); chars_out++;
        }
        if (!right_align && digits < width) {
          for (u64_t i = 0; i < width - digits; i++) {
            putc(' '); chars_out++;
          }
        }
      } break;

      case 'i': /* Decimal integer */
      case 'd': {

        i64_t val = (i64_t)va_arg(args, long long);
        /*if (length == 8) val = va_arg(args, long long);
        else val = va_arg(args, int);*/

        u64_t digits = 0;
        u64_t n = ABS(val);
        while (n) { n /= 10; digits++; }
        if (digits == 0) digits = 1;

        u64_t len = digits;
        if (positive_sign || val < 0) len++;

        if (zero_pad) {
          if (val < 0) { putc('-'); chars_out++; }
          else if (positive_sign) { putc(positive_sign); chars_out++; }
        }
        if (right_align && len < width) {
          for(u64_t i = 0; i < width - len; i++) {
            putc(zero_pad ? '0' : ' '); chars_out++;
          }
        }
        if (!zero_pad) {
          if (val < 0) { putc('-'); chars_out++; }
          else if (positive_sign) { putc(positive_sign); chars_out++; }
        }
        for (u64_t i = 0; i < digits; i++) {
          u64_t v = ABS(val);
          for (u64_t j = 0; j < digits - i - 1; j++) v /= 10;
          putc('0' + (v % 10)); chars_out++;
        }
        if (!right_align && len < width) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(' '); chars_out++;
          }
        }

        cur++;
      }  break;


      case 'p': /* Pointer */
        cur++;

        u64_t val = (u64_t)va_arg(args, void *);

        if (right_align && width > 18) {
          for (u64_t i = 0; i < width - 18; i++) {
            putc(' '); chars_out++;
          }
        }
        putc('0'); putc('x'); chars_out += 2;
        for (u64_t i = 0; i < 16; i++) {
          u8_t x = (val >> ((15-i)*4)) & 0xf;
          if (x < 10) { putc('0'+x); chars_out++; }
          else { putc('a'+x-10); chars_out++; }
        }
        if (!right_align && width > 18) {
          for (u64_t i = 0; i < width - 18; i++) {
            putc(' '); chars_out++;
          }
        }

        break;

      case 'o': /* Octal - lowercase */
        goto oct_out;
      case 'O': /* Octal - uppercase */
        capital = true;
        goto oct_out;
      oct_out: {
        cur++;

        u64_t val = (u64_t)va_arg(args, unsigned long long);
        /*if (length == 8) val = (u64_t)va_arg(args, unsigned long long);
        else val = (u64_t)va_arg(args, unsigned int);*/

        u64_t digits = 0;
        u64_t n = val;
        while (n) { n >>= 3; digits++; }
        if (digits == 0) digits = 1;

        u64_t len = digits;
        if (base_prefix) len += 2;

        if (right_align && len < width && !zero_pad) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(' '); chars_out++;
          }
        }
        if (base_prefix) {
          putc('0'); putc(capital ? 'O' : 'o'); chars_out += 2;
        }
        if (right_align && len < width && zero_pad) {
          for (u64_t i = 0; i < width - len; i++) {
            putc('0'); chars_out++;
          }
        }
        for (u64_t i = 0; i < digits; i++) {
          u8_t x = (val >> ((digits-i-1)*3)) & 0x7;
          putc('0'+x); chars_out++;
        }
        if (!right_align && len < width) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(zero_pad ? '0' : ' '); chars_out++;
          }
        }
      } break;

      case 'x': /* Hex - lowercase */
        goto hex_out;
      case 'X': /* Hex - uppercase */
        capital = true;
        goto hex_out;
      hex_out: {
        cur++;

        u64_t val = (u64_t)va_arg(args, unsigned long long);
        /*if (length == 8) val = (u64_t)va_arg(args, unsigned long long);
        else val = (u64_t)va_arg(args, unsigned int);*/

        u64_t digits = 0;
        u64_t n = val;
        while (n) { n >>= 4; digits++; }
        if (digits == 0) digits = 1;

        u64_t len = digits;
        if (base_prefix) len += 2;

        if (right_align && len < width && !zero_pad) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(' '); chars_out++;
          }
        }
        if (base_prefix) {
          putc('0'); putc(capital ? 'X' : 'x'); chars_out += 2;
        }
        if (right_align && len < width && zero_pad) {
          for (u64_t i = 0; i < width - len; i++) {
            putc('0'); chars_out++;
          }
        }
        for (u64_t i = 0; i < digits; i++) {
          u8_t x = (val >> ((digits-i-1)*4)) & 0xf;
          if (x < 10) { putc('0'+x); chars_out++; }
          else { putc((capital ? 'A' : 'a')+x-10); chars_out++; }
        }
        if (!right_align && len < width) {
          for (u64_t i = 0; i < width - len; i++) {
            putc(zero_pad ? '0' : ' '); chars_out++;
          }
        }
      } break;

      case 'f': /* Float - lowercase */
        goto float_out;
      case 'F': /* Float - uppercase */
        capital = true;
        goto float_out;
      float_out:
        cur++;
        /* TODO: float */
        break;

      default:
        return -1; /* Unrecognised type specifier */
        break;
    }
  }
  va_end(args);
  return chars_out;
}
