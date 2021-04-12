//
// Created by Karl on 4/11/2021.
//

#ifndef RT_IMPL_COMMON_H
#define RT_IMPL_COMMON_H

#include <stdint.h>

typedef double real_t;

typedef struct {
    real_t d[3];
} vec3_t;

typedef union {
    uint8_t d[4];
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t ignored;
    } rgb;
} pix_t;

#define PTR *
#define PTRC const*
#define CPTR *const

#endif //RT_IMPL_COMMON_H
