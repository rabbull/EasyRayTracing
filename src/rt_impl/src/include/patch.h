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

bool_t hit(patch_t PTRC patch, ray_t PTRC ray, real_t *dist, vec3_t *hit_point);

void print_patch(patch_t PTRC patch, char PTRC name, char PTRC prefix);

#endif //RT_IMPL_PATCH_H
