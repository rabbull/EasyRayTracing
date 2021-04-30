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
    vector_t *v, *w;
    real_t val;
    int i;

    v = vector_new(sizeof(real_t), 20);
    w = vector_new(sizeof(real_t), 20);
    for (i = 0; i < 20; ++i) {
        val = i;
        vector_append(v, &val);
        val = pow(-1, i) * i;
        vector_append(w, &val);
    }

    for (i = 0; i < v->length; ++i) {
        val = *(real_t *) vector_at(v, i);
        printf("%.1lf, ", val);
    }
    printf("\n");
    for (i = 0; i < w->length; ++i) {
        val = *(real_t *) vector_at(w, i);
        printf("%.1lf, ", val);
    }
    printf("\n");

    vector_sort_by_weights(v, w->payload);

    for (i = 0; i < v->length; ++i) {
        val = *(real_t *) vector_at(v, i);
        printf("%.1lf, ", val);
    }
    printf("\n");
    for (i = 0; i < w->length; ++i) {
        val = *(real_t *) vector_at(w, i);
        printf("%.1lf, ", val);
    }
    printf("\n");
}
