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

typedef double real_t;

static const real_t real_inf = 1e100;

bool_t eq(real_t p, real_t q);

real_t real_cos(real_t rad);

real_t real_sin(real_t rad);

void* ptr_offset(void* ptr, int64_t offset);

#define PTRC const*
#define CPTR *const
#define CPTRC const*const

#endif //RT_IMPL_UTILS_H
