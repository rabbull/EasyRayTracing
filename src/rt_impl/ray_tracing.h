//
// Created by 榨汁机 on 4/11/2021.
//

#ifndef RT_IMPL_RAY_TRACING_H
#define RT_IMPL_RAY_TRACING_H

#include "common.h"
#include "pixel.h"

typedef struct {
    vec3_t origin;
    vec3_t direction;
} ray_t;

typedef struct {
    vec3_t origin;

    // [roll, pitch, yaw] angle, in rad
    vec3_t orientation;
} observer_t;

typedef struct {
    size_t res_x;
    size_t res_y;
    real_t width;
    real_t height;
    pix_t* data;
} canvas_t;

typedef struct {
    observer_t observer;
    canvas_t canvas;
    real_t focal_distance;
} camera_t;

typedef struct {

} scene_t;

void *rt_impl(camera_t CPTRC camera, scene_t CPTRC scene);

#endif //RT_IMPL_RAY_TRACING_H
