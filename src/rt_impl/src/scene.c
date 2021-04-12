//
// Created by karl on 4/12/21.
//

#include <stdarg.h>
#include <memory.h>

#include "include/scene.h"

void combine(size_t n, scene_t CPTR scene, ...) {
    size_t i;
    size_t curr;
    size_t tot = 0;
    scene_t *next;

    va_list args;
    va_start(args, scene);

    for (i = 0; i < n; ++i) {
        tot += scene->num_patches;
    }
    curr = scene->num_patches;
    scene->num_patches = tot;
    scene->patches = realloc(scene->patches, tot * sizeof(patch_t));
    for (i = 1; i < n; ++i) {
        next = va_arg(args, scene_t CPTR);
        memcpy(scene->patches + curr, next->patches,
               next->num_patches * sizeof(patch_t));
        curr += next->num_patches;
        free(next->patches);
    }

    va_end(args);
}