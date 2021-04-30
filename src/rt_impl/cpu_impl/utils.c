//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <utils.h>

bool_t eq(real_t p, real_t q) {
    return fabs(p - q) < real_esp;
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

real_t real_abs(real_t v) {
    return (((v >= 0) << 1) - 1) * v;
}

real_t real_rand() {
    static bool_t initialized = FALSE;
    if (!initialized) {
        srand(time(NULL));
        initialized = TRUE;
    }
    return (real_t) rand() / RAND_MAX;
}

void real_swap(real_t *p, real_t *q) {
    real_t tmp;

    tmp = *p;
    *p = *q;
    *q = tmp;
}

void *ptr_offset(void *ptr, int64_t offset) {
    return (byte_t *) ptr + offset;
}
