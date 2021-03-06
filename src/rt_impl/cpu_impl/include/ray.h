//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_RAY_H
#define RT_IMPL_RAY_H

#include "linalg.h"

typedef struct {
    vec3_t origin;
    vec3_t direction;
} ray_t;

void ray_print(ray_t PTRC ray, char PTRC name);

#endif //RT_IMPL_RAY_H
