//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_PIXEL_H
#define RT_IMPL_PIXEL_H

#include <stdint.h>

typedef union {
    uint8_t d[4];
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t ignored;
    } rgb;
} pix_t;

#endif //RT_IMPL_PIXEL_H
