//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_UTILS_H
#define RT_IMPL_UTILS_H

#include <stdint.h>

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef unsigned char byte_t;

byte_t byte_min(byte_t p, byte_t q);
byte_t byte_max(byte_t p, byte_t q);

#define PTRC const*
#define CPTR *const
#define CPTRC const*const

#ifdef DOUBLE_PRECISION
typedef double real_t;
static const real_t real_inf = 1e20;
static const real_t real_eps = 1e-20;
#else
typedef float real_t;
static const real_t real_inf = 1e10f;
static const real_t real_eps = 1e-10f;
#endif

real_t real_cos(real_t rad);

real_t real_sin(real_t rad);

real_t real_abs(real_t v);

real_t real_max(real_t p, real_t q);

real_t real_min(real_t p, real_t q);

real_t real_rand();

void real_swap(real_t *p, real_t *q);

void *ptr_offset(void *ptr, int64_t offset);

bool_t str_startswith(char PTRC str, char PTRC target);

#endif //RT_IMPL_UTILS_H
