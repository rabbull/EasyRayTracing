//
// Created by karl on 4/12/21.
//

#include <stdio.h>

#include <mkl.h>
#include <math.h>
#include "src/include/alg.h"
#include "src/include/pixel.h"
#include "src/include/patch.h"


int main(int argc, char **argv) {
    patch_t patch = {
            .normal = {0, 0, 0},
            .vertices = {{0, 0, 0},
                         {0, 1, 0},
                         {0, 1, 1}}
    };
    ray_t ray = {
            .origin = {1, 2, 9},
            .direction = {1, 1, 0}
    };
    vec3_t hit_point = {0};
    pix_t color = {0};
    int flag = hit(&patch, &ray, &hit_point, &color, NULL, NULL, NULL, NULL);
    printf("%d\n", flag);
    print_vec3(&hit_point, "hit_point");
}
