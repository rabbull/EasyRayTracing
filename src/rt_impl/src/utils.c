//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdio.h>

#include "include/utils.h"

static const real_t EPS = 1e-6;

bool_t eq(real_t p, real_t q) {
    return fabs(p - q) < EPS;
}

real_t real_cos(real_t rad) {
    if (sizeof(real_t) == sizeof(double)) {
        return cos(rad);
    } else {
        return cosf(rad);
    }
}

real_t real_sin(real_t rad) {
    if (sizeof(real_t) == sizeof(double)) {
        return sin(rad);
    } else {
        return sinf(rad);
    }
}

void *ptr_offset(void *ptr, int64_t offset) {
    return (char *) ptr + offset;
}
