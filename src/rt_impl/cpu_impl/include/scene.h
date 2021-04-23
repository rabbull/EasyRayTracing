//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_SCENE_H
#define RT_IMPL_SCENE_H

#include <stdlib.h>

#include "patch.h"

typedef struct {
    vec3_t vertices[3];
} light_t;

typedef struct {
    size_t num_patches;
    patch_t *patches;

    size_t num_lights;
    light_t *lights;
} scene_t;

void combine(size_t n, scene_t *scene, ...);

bool_t fill_color(pix_t *const pix, ray_t *const ray, scene_t *const scene,
                  size_t depth, size_t max_depth);

void print_scene(scene_t PTRC scene, char PTRC name, char PTRC prefix);

#endif //RT_IMPL_SCENE_H