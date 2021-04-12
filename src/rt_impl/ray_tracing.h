//
// Created by 榨汁机 on 4/11/2021.
//

#ifndef RT_IMPL_RAY_TRACING_H
#define RT_IMPL_RAY_TRACING_H

#include "common.h"

typedef struct {
    vec3_t origin;
    vec3_t orient;
} ray_t;

typedef struct {
    vec3_t origin;
} observer_t;

typedef struct {
    size_t width;
    size_t height;
    pix_t* data;
} canvas_t;

typedef struct {

} scene_t;

#endif //RT_IMPL_RAY_TRACING_H
