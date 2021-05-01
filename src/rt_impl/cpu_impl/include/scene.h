//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_SCENE_H
#define RT_IMPL_SCENE_H

#include <stdlib.h>

#include "patch.h"
#include "material.h"

typedef struct {
    vec3_t vertices[3];
} light_t;

typedef struct {
    size_t num_patches;
    patch_t *patches;
    material_t **patch_material;

    size_t num_lights;
    light_t *lights;

    size_t num_materials;
    material_t *materials;
} scene_t;

void combine(size_t n, scene_t *scene, ...);

bool_t fill_color(pix_t* pix, ray_t PTRC ray, scene_t PTRC scene,
                  size_t depth, size_t max_depth,
                  char PTRC method, void* additional_args, bool_t verbose);

void scene_print(scene_t PTRC scene, char PTRC name, char PTRC prefix);

#endif //RT_IMPL_SCENE_H
