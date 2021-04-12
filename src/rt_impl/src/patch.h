//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_PATCH_H
#define RT_IMPL_PATCH_H

#include "utils.h"
#include "ray.h"

typedef struct {
    vec3_t vertices[3];
    vec3_t normal;
} patch_t;

int hit(patch_t PTRC patch, ray_t PTRC ray, vec3_t *hit_point,
        int *reflected, ray_t *reflection,
        int *transparent, ray_t *transparency);

#endif //RT_IMPL_PATCH_H
