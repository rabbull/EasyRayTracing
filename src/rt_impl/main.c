//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <cblas.h>

#include "cpu_impl/include/camera.h"
#include "cpu_impl/include/vector.h"

//int main(int argc, char **argv) {
//    camera_t camera = {
//            .focal_length = 1.0,
//            .observer = {
//                    .origin = {0, 0, 0},
//                    .orientation = {0, M_PI / 2, 0}
//            },
//            .canvas = {
//                    .res_x = 1920,
//                    .res_y = 1080,
//                    .height = 192,
//                    .width = 108,
//                    .data = calloc(sizeof(byte_t), 1920 * 1080 * 4)
//            }
//    };
//    scene_t scene = {
//            .num_lights = 10,
//            .lights = calloc(sizeof(light_t), 10),
//            .num_patches = 100,
//            .patches = calloc(sizeof(patch_t), 100),
//    };
//
//    observe(&camera, &scene);
//}

int main() {
    ray_t ray = {
            .origin = {0, 0, 0},
            .direction = {.1, .1, .1}
    };
    patch_t patch = {
            .vertices = {
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1},
            },
            .normal = {-1, -1, -1}
    };
    bool_t hit;
    real_t dist;
    vec3_t hit_point;

    hit = patch_hit(&patch, &ray, &dist, &hit_point);
    printf("hit: %d\n", hit);
    printf("dist: %.1lf\n", dist);
    print_vec3(&hit_point, NULL);
}
