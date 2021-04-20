//
// Created by 榨汁机 on 4/19/2021.
//

#include <stdio.h>

#include <cblas.h>
#include <lapacke.h>

#include "include/linalg.h"

void print_vec3(vec3_t CPTRC vec, char CPTRC name) {
    int i;

    if (name != NULL) {
        printf("%s = ", name);
    }
    printf("[");
    for (i = 0; i < 3; ++i) {
        printf(&", %.3lf"[(i == 0) << 1u], vec->d[i]);
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
            printf(&", %.3lf"[(j == 0) << 1u], mat->r[i].d[j]);
        }
        printf("\n");
    }
    printf("]\n");
}

void get_rotation_matrix(mat3_t CPTR rot, real_t const roll,
                         real_t const pitch, real_t const yaw) {
    real_t cr, sr, cp, sp, cy, sy;
    mat3_t mat_roll = {0}, mat_pitch = {0}, mat_yaw = {0}, tmp = {0};

    cr = real_cos(roll);
    sr = real_sin(roll);
    cp = real_cos(pitch);
    sp = real_sin(pitch);
    cy = real_cos(yaw);
    sy = real_sin(yaw);

    mat3_populate(mat_roll,
                  1, 0, 0,
                  0, cr, -sr,
                  0, sr, cr);
    mat3_populate(mat_pitch,
                  cp, 0, sp,
                  0, 1, 0,
                  -sp, 0, cp);
    mat3_populate(mat_yaw,
                  cy, -sy, 0,
                  sy, cy, 0,
                  0, 0, 1);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                3, 3, 3, 1, &mat_yaw, 3, &mat_pitch, 3, 0, &tmp, 3);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                3, 3, 3, 1, &tmp, 3, &mat_roll, 3, 0, rot, 3);
}


void vec3_left_multiply(vec3_t CPTR vec, mat3_t CPTRC mat, bool_t const trans) {
}

vec3_t PTRC vec3_zeros() {
    static const vec3_t zeros = {0, 0, 0};
    return &zeros;
}

mat3_t PTRC mat3_zeros() {
    static const mat3_t zeros = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    return &zeros;
}