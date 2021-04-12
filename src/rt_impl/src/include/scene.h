//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_SCENE_H
#define RT_IMPL_SCENE_H

#include <stdlib.h>

#include "patch.h"

typedef struct {
    size_t num_patches;
    patch_t *patches;
} scene_t;

void combine(size_t n, scene_t* scene, ...);

#endif //RT_IMPL_SCENE_H
