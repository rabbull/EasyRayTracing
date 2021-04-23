//
// Created by 榨汁机 on 4/22/2021.
//

#include <stdio.h>

#include <cuda_runtime.h>
#include <cublas_v2.h>

#include "include/utils.h"
#include "include/camera.h"
#include "include/scene.h"
#include "include/pixel.h"
#include "include/ray.h"


#define malloc_or_exit(err, var, size) do {                             \
    err = cudaMalloc(&var, size);                                       \
    if (err != cudaSuccess) {                                           \
        fprintf(stderr, "Failed to allocate var on device.\n");         \
        fprintf(stderr, "Error Code: %s\n", cudaGetErrorString(err));   \
        successfully_exit = FALSE;                                      \
        goto cleanup_and_exit;                                          \
    }                                                                   \
} while(0)

#define memcpy_or_exit(err, dst, src, size, kind) do {                  \
    err = cudaMemcpy(dst, src, size, kind);                             \
    if (err != cudaSuccess) {                                           \
        fprintf(stderr, "Failed to copy memory.\n");                    \
        fprintf(stderr, "Error Code: %s\n", cudaGetErrorString(err));   \
        successfully_exit = FALSE;                                      \
        goto cleanup_and_exit;                                          \
    }                                                                   \
} while (0)

#define free_if_is_not_null(ptr) do {   \
    if (ptr != NULL) {                  \
        cudaFree(ptr);                  \
    }                                   \
} while(0)

__global__ void calculate_rays(ray_t *rays, camera_t camera) {
    size_t const idx = blockDim.x * blockIdx.x + threadIdx.y;
    ray_t CPTR ray = rays + idx;

    canvas_t CPTRC canvas = &camera.canvas;
    real_t const pix_width = (real_t) canvas->width / canvas->res_x;
    real_t const pix_height = (real_t) canvas->height / canvas->res_y;
    real_t const half_width = canvas->width / 2;
    real_t const half_height = canvas->height / 2;
    size_t const u = idx % canvas->res_x;
    size_t const v = idx / canvas->res_x;

    ray->origin = camera.observer.origin;
    ray->direction.d[0] = 0;
    ray->direction.d[1] = half_width - u * pix_width;
    ray->direction.d[2] = half_height - v * pix_height;
}


int cuda_rt_impl(camera_t CPTRC _camera, scene_t CPTR _scene) {
    cudaError_t err;
    bool_t successfully_exit = TRUE;

    size_t s;
    size_t num_blocks_per_grid;
    size_t num_threads_per_block;

    pix_t *d_canvas = NULL;
    patch_t *d_patches = NULL;
    light_t *d_lights = NULL;

    real_t roll, pitch, yaw;
    mat3_t rotation;
    mat3_t *d_rot = NULL;

    size_t num_rays;
    ray_t *d_rays = NULL;

    // allocate canvas on device
    s = _camera->canvas.res_x * _camera->canvas.res_y;  // size of the canvas
    malloc_or_exit(err, d_canvas, s);

    // copy scene to device
    s = _scene->num_patches * sizeof(patch_t);
    malloc_or_exit(err, d_patches, s);
    memcpy_or_exit(err, d_patches, _scene->patches, s, cudaMemcpyHostToDevice);
    s = _scene->num_lights * sizeof(light_t);
    malloc_or_exit(err, d_lights, s);
    memcpy_or_exit(err, d_lights, _scene->lights, s, cudaMemcpyHostToDevice);

    // calculate rotation matrix, copy it to device and construct cuBLAS matrix
    roll = _camera->observer.orientation.d[0];
    pitch = _camera->observer.orientation.d[1];
    yaw = _camera->observer.orientation.d[2];
    get_rotation_matrix(&rotation, roll, pitch, yaw);
    s = sizeof(mat3_t);
    malloc_or_exit(err, d_rot, s);
    memcpy_or_exit(err, d_rot, &rotation, s, cudaMemcpyHostToDevice);

    // allocate rays on device
    num_rays = _camera->canvas.res_x * _camera->canvas.res_y;
    s = num_rays * sizeof(ray_t);
    malloc_or_exit(err, d_rays, s);

    num_threads_per_block = 1024;
    num_blocks_per_grid =
            (num_rays + num_threads_per_block - 1) / num_threads_per_block;
    calculate_rays<<<num_blocks_per_grid, num_threads_per_block>>>(
            d_rays, *_camera
    );

    cleanup_and_exit:
    free_if_is_not_null(d_canvas);
    free_if_is_not_null(d_patches);
    free_if_is_not_null(d_lights);
    free_if_is_not_null(d_rot);
    free_if_is_not_null(d_rays);
    return successfully_exit ? EXIT_SUCCESS : EXIT_FAILURE;
}
