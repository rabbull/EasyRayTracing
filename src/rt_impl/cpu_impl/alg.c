//
// Created by Karl on 4/11/2021.
//

#include <stdio.h>

#include <cblas.h>

#include "include/alg.h"

void *rt_impl(camera_t CPTRC camera, scene_t CPTR scene) {
    canvas_t CPTRC canvas = &camera->canvas;
    observer_t CPTRC observer = &camera->observer;
    real_t const pix_width = (real_t) canvas->width / canvas->res_x;
    real_t const pix_height = (real_t) canvas->height / canvas->res_y;
    real_t const half_width = canvas->width / 2;
    real_t const half_height = canvas->height / 2;

    size_t u, v;
    size_t offset;
    size_t progress = 0;

    ray_t *rays;
    vec3_t *directions;

    real_t roll = observer->orientation.d[0];
    real_t pitch = observer->orientation.d[1];
    real_t yaw = observer->orientation.d[2];
    mat3_t rot;
    get_rotation_matrix(&rot, roll, pitch, yaw);
    rays = calloc(sizeof(ray_t), canvas->res_x * canvas->res_y);
    directions = calloc(sizeof(vec3_t), canvas->res_x * canvas->res_y);
#pragma omp parallel for private(u, offset)
    for (v = 0; v < canvas->res_y; ++v) {
        offset = canvas->res_x * v;
        for (u = 0; u < canvas->res_x; ++u) {
            cblas_dcopy(3, &observer->origin, 1, &rays[offset + u].origin, 1);
            directions[offset + u].d[0] = camera->focal_length;
            directions[offset + u].d[1] = half_width - u * pix_width;
            directions[offset + u].d[2] = half_height - v * pix_height;
        }
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                    canvas->res_x, 3, 3, 1, directions + offset, 3, &rot, 3, 0,
                    ptr_offset(rays + offset, offsetof(ray_t, direction)), 6);
    }
    free(directions);

#pragma omp parallel for private(u) shared(progress)
    for (v = 0; v < canvas->res_x * canvas->res_y; ++v) {
        fill_color(canvas->data + v, rays + v, scene, 0, 3);
#pragma omp critical
        {
            progress += 1;
        }
        if (progress % 1000 == 0) {
            printf("progress: %zu%%\n",
                   progress * 100 / (canvas->res_x * canvas->res_y));
        }
    }
    free(rays);

    return NULL;
}
