//
// Created by 榨汁机 on 4/19/2021.
//

#include <stdio.h>

#include <cblas.h>

#include <linalg.h>

void print_vec3(vec3_t CPTRC vec, char CPTRC name) {
    int i;

    if (name != NULL) {
        printf("%s = ", name);
    }
    printf("[");
    for (i = 0; i < 3; ++i) {
        printf(&", %.5lf"[(i == 0) << 1u], vec->d[i]);
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
            printf(&", %.5lf"[(j == 0) << 1u], mat->r[i].d[j]);
        }
        printf("\n");
    }
    printf("]\n");
}

void mat3_transpose(mat3_t CPTR mat, mat3_t CPTR out) {
    real_t *mat_d = (real_t *) mat;
    real_t *out_d = (real_t *) out;
    size_t i, j;

    if (out != NULL) {
        for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
                out_d[i * 3 + j] = mat_d[(2 - i) * 3 + 2 - j];
            }
        }
    } else {
        for (i = 0; i < 3; ++i) {
            for (j = i + 1; j < 3; ++j) {
                real_swap(mat_d + i * 3 + j, mat_d + (2 - i) * 3 + 2 - j);
            }
        }
    }
}

void mat3_mm(mat3_t CPTRC mat_1, mat3_t CPTRC mat_2, mat3_t CPTR out) {
    size_t i, j, k;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            mat_at(*out, i, j) = 0;
            for (k = 0; k < 3; ++k) {
                mat_at(*out, i, j) +=
                        mat_at(*mat_1, i, k) * mat_at(*mat_2, k, j);
            }
        }
    }
}

void get_rotation_matrix(mat3_t CPTR rot, real_t const roll,
                         real_t const pitch, real_t const yaw) {
    real_t cr, sr, cp, sp, cy, sy;
    real_t tmp[9] = {0};
    mat3_t mat_roll = {0}, mat_pitch = {0}, mat_yaw = {0};

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
                3, 3, 3, 1, &mat_yaw, 3, &mat_pitch, 3, 0, tmp, 3);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                3, 3, 3, 1, tmp, 3, &mat_roll, 3, 0, rot, 3);
}

vec3_t PTRC vec3_zeros() {
    static const vec3_t zeros = {0, 0, 0};
    return &zeros;
}

mat3_t PTRC mat3_zeros() {
    static const mat3_t zeros = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    return &zeros;
}

void vec3_rand_unit(vec3_t *u) {
    size_t i;

    for (i = 0; i < 3; ++i) {
        u->d[i] = real_rand();
    }
    cblas_dscal(3, 1 / cblas_dnrm2(3, u, 1), u, 1);
}

void vec3_avg(vec3_t CPTR out, vec3_t CPTRC vectors, size_t const n) {
    size_t i;
    real_t inv_n = 1. / n;

    vec3_copy(out, vec3_zeros());
    for (i = 0; i < n; ++i) {
        vec3_axpy(inv_n, vectors + i, out);
    }
}

real_t vec3_cos_intersection_angle(vec3_t PTRC p, vec3_t PTRC q) {
    return vec3_dot(p, q) / vec3_norm2(p) / vec3_norm2(q);
}

void vec3_copy(vec3_t CPTR dst, vec3_t CPTRC src) {
#ifdef DOUBLE_PRECISION
    cblas_dcopy(3, src, 1, dst, 1);
#else
    cblas_scopy(3, src, 1, dst, 1);
#endif
}

void vec3_scale(vec3_t CPTR v, real_t alpha) {
#ifdef DOUBLE_PRECISION
    cblas_dscal(3, alpha, v, 1);
#else
    cblas_sscal(3, alpha, v, 1);
#endif
}

void vec3_axpy(real_t alpha, vec3_t CPTRC x, vec3_t CPTR y) {
#ifdef DOUBLE_PRECISION
    cblas_daxpy(3, alpha, x, 1, y, 1);
#else
    cblas_saxpy(3, alpha, x, 1, y, 1);
#endif
}

void vec3_plus(vec3_t *dst, vec3_t PTRC p, vec3_t PTRC q) {
    vec3_copy(dst, p);
    vec3_axpy(1, q, dst);
}

void vec3_minus(vec3_t CPTR dst, vec3_t CPTRC p, vec3_t CPTRC q) {
    vec3_copy(dst, p);
    vec3_axpy(-1, q, dst);
}

real_t vec3_dot(vec3_t CPTRC p, vec3_t CPTRC q) {
#ifdef DOUBLE_PRECISION
    return cblas_ddot(3, p, 1, q, 1);
#else
    return cblas_sdot(3, p, 1, q, 1);
#endif
}

void vec3_cross(vec3_t CPTR dst, vec3_t CPTRC p, vec3_t CPTRC q) {
    vec_at(*dst, 0) =
            vec_at(*p, 1) * vec_at(*q, 2) - vec_at(*p, 2) * vec_at(*q, 1);
    vec_at(*dst, 1) =
            vec_at(*p, 2) * vec_at(*q, 0) - vec_at(*p, 0) * vec_at(*q, 2);
    vec_at(*dst, 2) =
            vec_at(*p, 0) * vec_at(*q, 1) - vec_at(*p, 1) * vec_at(*q, 0);
}

real_t vec3_norm2(vec3_t CPTRC vec) {
#ifdef DOUBLE_PRECISION
    return cblas_dnrm2(3, vec, 1);
#else
    return cblas_snrm2(3, vec, 1);
#endif
}
