//
// Created by 榨汁机 on 4/19/2021.
//

#ifndef RT_IMPL_LINALG_H
#define RT_IMPL_LINALG_H

#include "utils.h"

typedef struct {
    real_t d[3];
} vec3_t;

typedef struct {
    vec3_t r[3];
} mat3_t;

#define vec_at(vec, index) ((vec).d[index])
#define mat_at(mat, row, col) ((mat).r[row].d[col])

#define mat3_populate(mat, v0, v1, v2, v3, v4, v5, v6, v7, v8) do { \
    mat_at((mat), 0, 0) = v0;                                       \
    mat_at((mat), 0, 1) = v1;                                       \
    mat_at((mat), 0, 2) = v2;                                       \
    mat_at((mat), 1, 0) = v3;                                       \
    mat_at((mat), 1, 1) = v4;                                       \
    mat_at((mat), 1, 2) = v5;                                       \
    mat_at((mat), 2, 0) = v6;                                       \
    mat_at((mat), 2, 1) = v7;                                       \
    mat_at((mat), 2, 2) = v8;                                       \
} while (0)

void print_vec3(vec3_t PTRC vec, char PTRC name);

void print_mat3(mat3_t PTRC mat, char PTRC name);

void get_rotation_matrix(mat3_t *rot, real_t roll, real_t pitch, real_t yaw);

vec3_t PTRC vec3_zeros();

mat3_t PTRC mat3_zeros();

void vec3_rand_unit(vec3_t *u);

void vec3_avg(vec3_t *out, vec3_t PTRC vectors, size_t n);

real_t vec3_cos_intersection_angle(vec3_t PTRC p, vec3_t PTRC q);

void vec3_copy(vec3_t *dst, vec3_t PTRC src);

void vec3_axpy(real_t alpha, vec3_t PTRC x, vec3_t *y);

void vec3_scale(vec3_t *v, real_t alpha);

void vec3_plus(vec3_t *dst, vec3_t PTRC p, vec3_t PTRC q);

void vec3_minus(vec3_t *dst, vec3_t PTRC p, vec3_t PTRC q);

real_t vec3_dot(vec3_t PTRC p, vec3_t PTRC q);

real_t vec3_norm2(vec3_t PTRC vec);

void vec3_cross(vec3_t *dst, vec3_t PTRC, vec3_t PTRC);

void mat3_transpose(mat3_t *mat, mat3_t *out);

void mat3_mm(mat3_t PTRC mat_1, mat3_t PTRC mat_2, mat3_t *out);

#endif //RT_IMPL_LINALG_H
