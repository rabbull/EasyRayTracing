//
// Created by 榨汁机 on 4/27/2021.
//

#ifndef RT_IMPL_MATERIAL_H
#define RT_IMPL_MATERIAL_H

#include "linalg.h"

typedef struct {
    struct {
        vec3_t a;
        vec3_t d;
        vec3_t s;
    } k;
} material_t;

#endif //RT_IMPL_MATERIAL_H
