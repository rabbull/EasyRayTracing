//
// Created by karl on 4/12/21.
//

#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include <cblas.h>

#include <bvh.h>
#include <scene.h>
#include <vector.h>

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

static void find_nearest_patch(ray_t CPTRC ray, scene_t CPTRC scene,
                               patch_t PTRC*nearest_patch,
                               vec3_t CPTR hit_point,
                               material_t PTRC* mtl) {
    size_t i;
    patch_t *patch;
    vec3_t tmp_hit_point;
    real_t patch_dist;
    real_t nearest_patch_dist = real_inf;

    *nearest_patch = NULL;
    *mtl = NULL;

    for (i = 0; i < scene->num_patches; ++i) {
        patch = scene->patches + i;
        if (patch_hit(patch, ray, &patch_dist, &tmp_hit_point)) {
            if (patch_dist < nearest_patch_dist) {
                *nearest_patch = patch;
                nearest_patch_dist = patch_dist;
                cblas_dcopy(3, &tmp_hit_point, 1, hit_point, 1);
                *mtl = scene->patch_material[i];
            }
        }
    }
}

static void find_intersecting_nodes(ray_t CPTRC ray, vector_t CPTR nodes,
                                    bvh_tree_node_t CPTRC node) {
    if (!aabb_hit(&node->aabb, ray)) {
        return;
    }
    if (bvh_tree_node_is_leaf(node)) {
        vector_push_back(nodes, &node);
        return;
    }
    find_intersecting_nodes(ray, nodes, node->left);
    find_intersecting_nodes(ray, nodes, node->right);
}

static void find_nearest_patch_bvh(ray_t CPTRC ray, bvh_tree_t CPTRC tree,
                                   patch_t PTRC*nearest_patch,
                                   vec3_t CPTR hit_point,
                                   material_t PTRC* mtl) {
    size_t i, j;
    real_t dist, nearest_dist;
    bvh_tree_node_t *node;
    bvh_payload_t *payload;
    vec3_t tmp_hit_point;
    vector_t *intersecting_nodes;

    *nearest_patch = NULL;
    *mtl = NULL;
    intersecting_nodes = vector_new(sizeof(bvh_tree_node_t *), 0);

    find_intersecting_nodes(ray, intersecting_nodes, tree->root);
    if (intersecting_nodes->length == 0) {
        vector_destroy(intersecting_nodes);
        return;
    }

    for (i = 0; i < intersecting_nodes->length; ++i) {
        node = *(bvh_tree_node_t **) vector_at(intersecting_nodes, i);
        nearest_dist = real_inf;
        for (j = 0; j < node->payload_slice_size; ++j) {
            payload = node->payload_slice + j;
            if (patch_hit(payload->patch, ray, &dist, &tmp_hit_point)) {
                if (dist < nearest_dist) {
                    nearest_dist = dist;
                    *nearest_patch = payload->patch;
                    *mtl = payload->mtl;
                    *hit_point = tmp_hit_point;
                }
            }
        }
    }

    vector_destroy(intersecting_nodes);
}

bool_t fill_color(pix_t CPTR pix, ray_t CPTRC ray, scene_t CPTRC scene,
                  size_t const depth, size_t const max_depth,
                  char CPTRC method, void *additional_args) {
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

    patch_t PTRC patch;
    patch_t PTRC nearest_patch = NULL;
    real_t nearest_patch_dist = real_inf;

    material_t PTRC mtl = NULL;
    material_t const default_mtl = {
            .k = {
                    .a = {1, 1, 1},
                    .d = {1, 1, 1},
                    .s = {1, 1, 1}
            }
    };

    pix_t *pixels[8];
    real_t const weights[8] = {0, 10, 10};

    if (depth == max_depth) {
        pix->r = pix->g = pix->b = 0;
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

    if (method == NULL) {
        find_nearest_patch(ray, scene, &nearest_patch, &hit_point, &mtl);
    } else if (str_startswith(method, "bvh")) {
        find_nearest_patch_bvh(ray, (bvh_tree_t *) additional_args,
                               &nearest_patch, &hit_point, &mtl);
    } else {
        fprintf(stderr, "Method not supported: %s\n", method);
        return FALSE;
    }

    if (nearest_patch == NULL && nearest_light == NULL) {
        pix->r = pix->g = pix->b = 0;
        return FALSE;
    }
    if (nearest_light != NULL && nearest_light_dist < nearest_patch_dist) {
        pix->r = 255;
        pix->g = 255;
        pix->b = 255;
        return TRUE;
    }

    if (mtl == NULL) {
        mtl = &default_mtl;
    }
    for (i = 0; i < 3; ++i) {
        ambient.d[i] = (uint8_t) (255 * mtl->k.a.d[0]);
    }

    patch = nearest_patch;
    cos_diffuse_angle = real_abs(
            vec3_dot(&ray->direction, &patch->normal)
            / vec3_norm2(&ray->direction)
            / vec3_norm2(&patch->normal)
    );
    for (i = 0; i < 3; ++i) {
        diffuse.d[i] = (uint8_t) (255 * cos_diffuse_angle * mtl->k.d.d[i]);
    }

//    patch_reflect(nearest_patch, ray, &hit_point, &specular_ray);
//    fill_color(&specular, &specular_ray, scene, depth + 1, max_depth, method,
//               additional_args);
//    for (i = 0; i < 3; ++i) {
//        specular.d[i] *= mtl->k.d.d[i];
//    }

    pixels[0] = &ambient;
    pixels[1] = &diffuse;
//    pixels[2] = &specular;
    blend(pixels, weights, 2, pix);

    return TRUE;
}

void scene_print(scene_t CPTRC scene, char PTRC name, char PTRC prefix) {
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