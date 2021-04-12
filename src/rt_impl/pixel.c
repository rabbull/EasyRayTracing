//
// Created by karl on 4/12/21.
//

#include "common.h"
#include "pixel.h"

uint8_t pix_get_r(pix_t PTRC pix) {
    return pix->rgb.r;
}

uint8_t pix_get_g(pix_t PTRC pix) {
    return pix->rgb.g;
}

uint8_t pix_get_b(pix_t PTRC pix) {
    return pix->rgb.b;
}