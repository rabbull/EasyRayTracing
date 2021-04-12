//
// Created by Karl on 4/11/2021.
//

#include <stdlib.h>
#include <string.h>
#include <openblas.h>

#include "ray_tracing.h"

static int intersect(ray_t PTRC ray, scene_t PTRC scene) {
    return 0;
}

static void trace(ray_t PTRC ray, scene_t PTRC scene) {
}

static pix_t *at(canvas_t PTRC canvas, size_t x, size_t y) {
    return canvas->data + y * canvas->width + x;
}

void rt_impl(
        observer_t PTRC observer,
        canvas_t PTRC canvas,
        scene_t PTRC scene) {
    size_t x, y;
    size_t i, j;
    pix_t *curr;
    ray_t ray;

    for (y = 0; y < canvas->height; ++y) {
        for (x = 0; x < canvas->width; ++x) {
            curr = at(canvas, x, y);
            memcpy((void *) observer->origin.d, ray.origin.d,
                   sizeof(real_t) * 3);

        }
    }
}