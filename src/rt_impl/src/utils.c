//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdio.h>

#include "utils.h"

static const real_t EPS = 1e-3;

void print_vec3(vec3_t CPTRC vec, char CPTRC name) {
    int i;

    if (name != NULL) {
        printf("%s = ", name);
    }
    printf("[");
    for (i = 0; i < 3; ++i) {
        printf(&", %.1lf"[(i == 0) << 1u], vec->d[i]);
    }
    printf("]\n");
}

void print_mat3(mat3_t CPTRC mat, char CPTRC name) {
    int i, j;

    if (name != NULL) {
        printf("%s = ", name);
    }
    printf("[\n");
    for (i = 0; i < 3; ++i) {
        printf("\t");
        for (j = 0; j < 3; ++j) {
            printf(&", %.1lf"[(j == 0) << 1u], mat->r[i].d[j]);
        }
        printf("\n");
    }
    printf("]\n");
}

int eq(real_t p, real_t q) {
    return fabs(p - q) < EPS;
}
