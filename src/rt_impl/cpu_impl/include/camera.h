//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_CAMERA_H
#define RT_IMPL_CAMERA_H

#include "utils.h"
#include "pixel.h"
#include "linalg.h"

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
    pix_t *data;
} canvas_t;

typedef struct {
    observer_t observer;
    canvas_t canvas;
    real_t focal_length;
} camera_t;

#endif //RT_IMPL_CAMERA_H
