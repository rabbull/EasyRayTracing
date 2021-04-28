//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <cblas.h>

#include "cpu_impl/include/alg.h"

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
//    rt_impl(&camera, &scene);
//}


int main() {
    mat3_t A = {0}, B = {0}, C = {0};
    mat3_populate(A, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    mat3_populate(B, 2, 3, 4, 5, 6, 7, 8, 9, 0);
    print_mat3(&A, "A");
    print_mat3(&B, "B");
    print_mat3(&C, "C");
    mat3_mm(&A, &B, &C);
    print_mat3(&A, "A");
    print_mat3(&B, "B");
    print_mat3(&C, "C");
}
