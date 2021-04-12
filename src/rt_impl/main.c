//
// Created by karl on 4/12/21.
//

#include <stdio.h>

#include <mkl.h>
#include <math.h>
#include "ray_tracing.h"
#include "pixel.h"


int main(int argc, char **argv) {
    double rot[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double d[3] = {9, 7, 1};
    double res[3];

    pix_t buff[50] = {0};
    camera_t camera = {
            .focal_distance = 10,
            .canvas.res_x = 10,
            .canvas.res_y = 5,
            .canvas.width = 4,
            .canvas.height = 2,
            .canvas.data = buff,
            .observer.origin.d = {0, 0, 0},
            .observer.orientation.d = {0, 0, 0}
    };
    scene_t scene;
    rt_impl(&camera, &scene);
}
