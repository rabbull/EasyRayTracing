//
// Created by karl on 4/12/21.
//

#include <stdio.h>

#include "src/include/alg.h"

int main(int argc, char **argv) {
    patch_t patch = {
            .normal = {0, 0, 0},
            .vertices = {{0, 0, 0},
                         {0, 1, 0},
                         {0, 0, 1}}
    };
    ray_t ray = {
            .origin = {1, 0.1, 0.1},
            .direction = {-1, 0, 0}
    };

    real_t distance = 0;
    vec3_t hit_point = {0};
    int flag = hit(&patch, &ray, &distance, &hit_point);
    printf("%d %lf\n", flag, distance);
    print_vec3(&hit_point, "hit_point");
}
