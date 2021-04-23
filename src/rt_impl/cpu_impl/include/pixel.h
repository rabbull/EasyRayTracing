//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_PIXEL_H
#define RT_IMPL_PIXEL_H

#include <stdint.h>

#include "utils.h"

typedef union {
    uint8_t d[4];
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t ignored;
    } rgb;
} pix_t;

void pix_print(pix_t const* pixel, char const* name);

void blend(pix_t *const *pixels, real_t PTRC weights, size_t n, pix_t *output);

#endif //RT_IMPL_PIXEL_H