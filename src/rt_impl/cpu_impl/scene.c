//
// Created by karl on 4/12/21.
//

#include <stdarg.h>
#include <memory.h>
#include <stdio.h>
#include <cblas.h>

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

bool_t fill_color(pix_t CPTR pix, ray_t CPTR ray, scene_t CPTR scene,
                  size_t depth, size_t max_depth) {
    size_t i;
    vec3_t hit_point = {0};

    pix_t ambient = {0};
    pix_t diffuse = {0};
    real_t cos_diffuse_angle;
    pix_t specular = {0};
    ray_t specular_ray = {0};

    light_t *light;
    patch_t light_patch;
    light_t *nearest_light = NULL;
    real_t light_dist;
    real_t nearest_light_dist = real_inf;

    patch_t *patch;
    patch_t *nearest_patch = NULL;
    real_t patch_dist;
    real_t nearest_patch_dist = real_inf;

    material_t *mtl;
    material_t const default_mtl = {
            .k = {
                    .a = {1, 1, 1},
                    .d = {1, 1, 1},
                    .s = {1, 1, 1}
            }
    };

    pix_t *pixels[8];
    real_t const weights[8] = {10, 10, 0};

    if (depth == max_depth) {
        pix->rgb.r = pix->rgb.g = pix->rgb.b = 0;
        return FALSE;
    }

    for (i = 0; i < scene->num_lights; ++i) {
        light = scene->lights + i;
        cblas_dcopy(9, light->vertices, 1, light_patch.vertices, 1);
        if (patch_hit(&light_patch, ray, &light_dist, &hit_point)) {
            if (light_dist < nearest_light_dist) {
                nearest_light_dist = light_dist;
                nearest_light = light;
            }
        }
    }
    for (i = 0; i < scene->num_patches; ++i) {
        patch = scene->patches + i;
        if (patch_hit(patch, ray, &patch_dist, &hit_point)) {
            if (patch_dist < nearest_patch_dist) {
                nearest_patch_dist = patch_dist;
                nearest_patch = patch;
                mtl = scene->patch_material[i];
                if (mtl == NULL) {
                    mtl = &default_mtl;
                }
            }
        }
    }
    if (nearest_patch == NULL && nearest_light == NULL) {
        pix->rgb.r = pix->rgb.g = pix->rgb.b = 0;
        return FALSE;
    }
    if (nearest_light != NULL && nearest_light_dist < nearest_patch_dist) {
        pix->rgb.r = 255;
        pix->rgb.g = 255;
        pix->rgb.b = 255;
        return TRUE;
    }

    for (i = 0; i < 3; ++i) {
        ambient.d[i] = (uint8_t) (255 * mtl->k.a.d[0]);
    }

    patch = nearest_patch;
    cos_diffuse_angle =
            real_abs(cblas_ddot(3, &ray->direction, 1, &patch->normal, 1))
            / cblas_dnrm2(3, &ray->direction, 1)
            / cblas_dnrm2(3, &patch->normal, 1);
    for (i = 0; i < 3; ++i) {
        diffuse.d[i] = (uint8_t) (255 * cos_diffuse_angle * mtl->k.d.d[i]);
    }

    patch_reflect(nearest_patch, ray, &hit_point, &specular_ray);
    fill_color(&specular, &specular_ray, scene, depth + 1, max_depth);
    for (i = 0; i < 3; ++i) {
        specular.d[i] *= mtl->k.d.d[i];
    }

    pixels[0] = &ambient;
    pixels[1] = &diffuse;
    pixels[2] = &specular;
    blend(pixels, weights, 3, pix);

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
        patch_print(scene->patches + i, patch_name, "\t");
    }
}