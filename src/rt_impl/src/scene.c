//
// Created by karl on 4/12/21.
//

#include <stdarg.h>
#include <memory.h>
#include <stdio.h>

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

bool_t color(pix_t CPTR pix, ray_t CPTR ray, scene_t CPTR scene) {
    size_t i;
    bool_t flag;

    patch_t *patch;
    patch_t *nearest_patch = NULL;
    real_t distance;
    real_t nearest_distance = real_inf;

    for (i = 0; i < scene->num_patches; ++i) {
        patch = scene->patches + i;
        flag = hit(patch, ray, &distance, NULL);
        if (flag) {
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_patch = patch;
            }
        }
    }
    if (nearest_patch == NULL) {
        return FALSE;
    }
    pix->d[0] = 255;
    pix->d[1] = 0;
    pix->d[2] = 0;
    return TRUE;
}

void print_scene(scene_t CPTRC scene, char PTRC name, char PTRC prefix) {
    size_t i;
    char patch_name[64];

    if (name == NULL) {
        name = "scene";
    }
    if (prefix == NULL) {
        prefix = "";
    }

    printf("%s%s = \n", prefix, name);
    printf("\tnum_patches = %zu\n", scene->num_patches);
    for (i = 0; i < scene->num_patches; ++i) {
        sprintf(patch_name, "[%zu]", i);
        print_patch(scene->patches + i, patch_name, "\t");
    }
}