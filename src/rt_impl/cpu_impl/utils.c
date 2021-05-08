//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <utils.h>

real_t real_cos(real_t rad) {
#ifdef DOUBLE_PRECISION
        return cos(rad);
#else
        return cosf(rad);
#endif
}

real_t real_sin(real_t rad) {
#ifdef DOUBLE_PRECISION
    return sin(rad);
#else
    return sinf(rad);
#endif
}

real_t real_abs(real_t v) {
    return (((v >= 0) << 1) - 1) * v;
}

real_t real_max(real_t p, real_t q) {
    return p > q ? p : q;
}

real_t real_min(real_t p, real_t q) {
    return p < q ? p : q;
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

bool_t str_startswith(char CPTRC str, char CPTRC target) {
    char const *q = str;
    char const *p = target;

    while (*p != 0) {
        if (*p != *q) {
            return FALSE;
        }
        p += 1;
        q += 1;
    }
    return TRUE;
}
