//
// Created by Karl on 4/11/2021.
//

#include <math.h>

#include <cblas.h>
#include <stdio.h>

#include "include/alg.h"

static pix_t *at(canvas_t PTRC canvas, size_t x, size_t y) {
    return canvas->data + y * canvas->res_x + x;
}

static void get_rotation_matrix(real_t rot[], real_t alpha,
                                real_t beta, real_t gamma) {
    real_t (*c)(real_t);
    real_t (*s)(real_t);
    real_t ca, sa, cb, sb, cg, sg;
    real_t r1[9] = {0}, r2[9] = {0}, r3[9] = {0}, tmp[9];

    if (sizeof(real_t) == sizeof(double)) {
        c = cos;
        s = sin;
    } else if (sizeof(real_t) == sizeof(float)) {
        c = cosf;
        s = sinf;
    } else {
        return;
    }

    ca = c(alpha);
    sa = s(alpha);
    cb = c(beta);
    sb = s(beta);
    cg = c(gamma);
    sg = s(gamma);

    r1[0] = cg;
    r1[1] = sg;
    r1[3] = -sg;
    r1[4] = cg;
    r1[8] = 1;

    r2[0] = 1;
    r2[4] = cb;
    r2[5] = sb;
    r2[7] = -sb;
    r2[8] = cb;

    r3[0] = ca;
    r3[1] = sa;
    r3[3] = -sa;
    r3[4] = ca;
    r3[8] = 1;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                3, 3, 3, 1, r1, 3, r2, 3, 0, tmp, 3);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                3, 3, 3, 1, tmp, 3, r3, 3, 0, rot, 3);
}

void *rt_impl(camera_t CPTRC camera, scene_t CPTRC scene) {
    canvas_t CPTRC canvas = &camera->canvas;
    observer_t CPTRC observer = &camera->observer;
    size_t u, v;
    size_t i, j;
    pix_t *curr_pix;
    ray_t ray;
    vec3_t vec;

    real_t alpha = camera->observer.orientation.d[0];
    real_t beta = camera->observer.orientation.d[1];
    real_t gamma = camera->observer.orientation.d[2];

    real_t rot[9];

    get_rotation_matrix(rot, alpha, beta, gamma);
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf(&"\n%.1lf "[!(j == 0 && i != 0)], rot[i * 3 + j]);
        }
    }
    printf("\n\n");

    for (v = 0; v < canvas->res_y; ++v) {
        for (u = 0; u < canvas->res_x; ++u) {
            curr_pix = at(canvas, u, v);

            cblas_dcopy(3, observer->origin.d, 1, ray.origin.d, 1);

            ray.direction.d[0] = 1;
            ray.direction.d[1] = ray.direction.d[2] = 0;

            ray.direction.d[0] = 0;
            ray.direction.d[1] =
                    canvas->width / 2 - u * canvas->width / canvas->res_x;
            ray.direction.d[2] =
                    canvas->height / 2 - v * canvas->height / canvas->res_y;

            vec.d[0] = camera->focal_distance;
            vec.d[1] = vec.d[2] = 0;

            cblas_daxpy(3, 1, ray.direction.d, 1, vec.d, 1);
            cblas_dgemv(CblasRowMajor, CblasNoTrans, 3, 3,
                        1, rot, 3, vec.d, 1, 0, ray.direction.d, 1);

            print_vec3(&ray.direction, "ray.direction");

            for (i = 0; i < scene->num_patches; ++i) {

            }
        }
    }
    return NULL;
}
