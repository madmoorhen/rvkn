/* Include guard */
#ifndef UTILS_BASE_H
#define UTILS_BASE_H

/* Includes */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Basic types */
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef float f32_t;
typedef double f64_t;

/* Size checks for basic types */
#define CHECK_TYPE(t, s) _Static_assert(sizeof(t)==s, #t " must be " #s " bytes")
CHECK_TYPE(u8_t, 1);
CHECK_TYPE(u16_t, 2);
CHECK_TYPE(u32_t, 4);
CHECK_TYPE(u64_t, 8);
CHECK_TYPE(i8_t, 1);
CHECK_TYPE(i16_t, 2);
CHECK_TYPE(i32_t, 4);
CHECK_TYPE(i64_t, 8);
CHECK_TYPE(f32_t, 4);
CHECK_TYPE(f64_t, 8);
#undef CHECK_TYPE

/* Inline assembly */
#define ASM __asm__ __volatile__

/* Minimum value */
#define MIN(a, b) ((a) > (b) ? (b) : (a))
/* Maximum value */
#define MAX(a, b) ((a) < (b) ? (b) : (a))
/* Clamp a value */
#define CLAMP(min, max, a) (MAX(min, MIN(v, max)))
/* Linear interpolation */
#define LERP(min, max, t) ((min) + (t)*((max)-(min)))
/* Absolute value */
#define ABS(a) ((a) < 0 ? -(a) : (a))
/* Array length */
#define ARRLEN(a) (sizeof(a)/sizeof(a[0]))

/* This many brackets is as close as I'm getting to lisp */
/* 16 bit endian switch */
#define ENDIAN16(v) ((((u16_t)(v))<<8)|(((u16_t)v)>>8))
/* 32 bit endian switch */
#define ENDIAN32(v) (((((u16_t)(v))<<24)&0xff000000)\
    |((((u16_t)(v))<<8 )&0x00ff0000)\
    |((((u16_t)(v))>>8 )&0x0000ff00)\
    |((((u16_t)(v))>>24)&0x000000ff))
/* 64 bit endian switch */
#define ENDIAN64(v) (((((u16_t)(v))<<56)&0xff00000000000000)\
    |((((u16_t)(v))<<40)&0x00ff000000000000)\
    |((((u16_t)(v))<<24)&0x0000ff0000000000)\
    |((((u16_t)(v))<<8)& 0x000000ff00000000)\
    |((((u16_t)(v))>>8)& 0x00000000ff000000)\
    |((((u16_t)(v))>>24)&0x0000000000ff0000)\
    |((((u16_t)(v))>>40)&0x000000000000ff00)\
    |((((u16_t)(v))>>56)&0x00000000000000ff))

/* String length, not including NULL terminator */
static u64_t strlen(const char *s) { u64_t l = 0; while (s[l]) l++; return l; }

#endif /* UTILS_BASE_H */
