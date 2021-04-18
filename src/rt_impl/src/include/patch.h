//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_PATCH_H
#define RT_IMPL_PATCH_H

#include "utils.h"
#include "ray.h"
#include "pixel.h"

typedef struct {
    vec3_t vertices[3];
    vec3_t normal;
} patch_t;

static real_t const OFF_TARGET = -1;

bool_t hit(patch_t *patch, ray_t PTRC ray, real_t *dist, vec3_t *hit_point);

#endif //RT_IMPL_PATCH_H
