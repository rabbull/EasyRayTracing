//
// Created by Karl on 4/11/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include <cblas.h>
#include <omp.h>

#include <camera.h>
#include <scene.h>
#include <bvh.h>
#include <vector.h>

void *observe(camera_t CPTRC camera, scene_t CPTR scene, char CPTRC method) {
    canvas_t CPTRC canvas = &camera->canvas;
    observer_t CPTRC observer = &camera->observer;
    real_t const pix_width = (real_t) canvas->width / canvas->res_x;
    real_t const pix_height = (real_t) canvas->height / canvas->res_y;
    real_t const half_width = canvas->width / 2;
    real_t const half_height = canvas->height / 2;

    size_t u, v;
    size_t offset;

    ray_t *rays = NULL;
    vec3_t *directions = NULL;

    real_t roll = observer->orientation.d[0];
    real_t pitch = observer->orientation.d[1];
    real_t yaw = observer->orientation.d[2];
    mat3_t rot;
    bvh_tree_t *tree = NULL;
    void *additional_args = NULL;

    size_t progress = 0;
    size_t percent, max_percent = 0;

    get_rotation_matrix(&rot, roll, pitch, yaw);
    rays = calloc(sizeof(ray_t), canvas->res_x * canvas->res_y);
    directions = calloc(sizeof(vec3_t), canvas->res_x * canvas->res_y);

    if (method == NULL) {
        // doing nothing
    } else if (str_startswith(method, "bvh")) {
        printf("constructing bvh tree..\n");
        if (str_startswith(method + 3, "middle")) {
            tree = bvh_tree_construct(scene, BVH_SPLIT_MIDDLE);
        } else if (str_startswith(method + 3, "sah")) {
            tree = bvh_tree_construct(scene, BVH_SPLIT_SAH);
        } else {
            fprintf(stderr, "Unrecognized method: %s\n", method);
            goto cleanup_and_exit;
        }
        additional_args = tree;
    } else {
        fprintf(stderr, "Unrecognized method: %s\n", method);
        goto cleanup_and_exit;
    }

#pragma omp parallel for private(u, offset)
    for (v = 0; v < canvas->res_y; ++v) {
        offset = canvas->res_x * v;
        for (u = 0; u < canvas->res_x; ++u) {
            vec3_copy(&rays[offset + u].origin, &observer->origin);
            directions[offset + u].d[0] = camera->focal_length;
            directions[offset + u].d[1] = half_width - (u + .5) * pix_width;
            directions[offset + u].d[2] = half_height - (v + .5) * pix_height;
        }
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                    canvas->res_x, 3, 3, 1, directions + offset, 3, &rot, 3, 0,
                    ptr_offset(rays + offset, offsetof(ray_t, direction)), 6);
    }

    printf("rendering..\n");
#pragma omp parallel for private(v) shared(method, additional_args)
    for (v = 0; v < canvas->res_x * canvas->res_y; ++v) {
        fill_color(canvas->data + v, rays + v, scene, 0, 1,
                   method, additional_args, v == 24 * canvas->res_x + 16);
#pragma omp critical
        {
            progress += 1;
            percent = progress * 100 / (canvas->res_x * canvas->res_y);
            if (percent > max_percent) {
                max_percent = percent;
//                printf("progress: %zu%%\n", max_percent);
            }
        }
        if (v == 24 * canvas->res_x + 16) {
            pix_print(canvas->data + v, "the pixel");
        }
    }

    cleanup_and_exit:
    printf("done, clearing..\n");
    free(directions);
    free(rays);
    if (tree != NULL) {
        bvh_tree_destroy(tree);
    }
    return NULL;
}
