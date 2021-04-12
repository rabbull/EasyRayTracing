//
// Created by Karl on 4/11/2021.
//

#ifndef RT_IMPL_COMMON_H
#define RT_IMPL_COMMON_H

#include <stdint.h>

//typedef float real_t;
typedef double real_t;

typedef struct {
    real_t d[3];
} vec3_t;

#define PTRC const*
#define CPTR *const
#define CPTRC const*const

#endif //RT_IMPL_COMMON_H
