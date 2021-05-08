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
                               material_t PTRC*mtl,
                               patch_t CPTRC ignore_patch) {
    size_t i;
    patch_t *patch;
    vec3_t tmp_hit_point;
    real_t patch_dist;
    real_t nearest_patch_dist = real_inf;

    *nearest_patch = NULL;
    if (mtl != NULL) {
        *mtl = NULL;
    }

    for (i = 0; i < scene->num_patches; ++i) {
        patch = scene->patches + i;
        if (patch == ignore_patch) {
            continue;
        }
        if (patch_hit(patch, ray, &patch_dist, &tmp_hit_point)) {
            if (patch_dist < nearest_patch_dist) {
                *nearest_patch = patch;
                nearest_patch_dist = patch_dist;
                if (hit_point != NULL) {
                    vec3_copy(hit_point, &tmp_hit_point);
                }
                if (mtl != NULL) {
                    *mtl = scene->patch_material[i];
                }
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
                                   material_t PTRC*mtl,
                                   patch_t CPTRC ignore_patch) {
    size_t i, j;
    real_t dist, nearest_dist;
    bvh_tree_node_t *node;
    bvh_payload_t *payload;
    vec3_t tmp_hit_point;
    vector_t *intersecting_nodes;

    *nearest_patch = NULL;
    if (mtl != NULL) {
        *mtl = NULL;
    }
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
            if (payload->patch == ignore_patch) {
                continue;
            }
            if (patch_hit(payload->patch, ray, &dist, &tmp_hit_point)) {
                if (dist < nearest_dist) {
                    nearest_dist = dist;
                    *nearest_patch = payload->patch;
                    if (mtl != NULL) {
                        *mtl = payload->mtl;
                    }
                    if (hit_point != NULL) {
                        *hit_point = tmp_hit_point;
                    }
                }
            }
        }
    }

    vector_destroy(intersecting_nodes);
}

bool_t fill_color(pix_t CPTR pix, ray_t CPTRC ray, scene_t CPTRC scene,
                  size_t const depth, size_t const max_depth,
                  char CPTRC method, void *args) {
    size_t i, j;
    int tmp;
    vec3_t hit_point = {0};
    light_t *light;
    patch_t *patch;
    pix_t ambient = {0};
    pix_t diffuse = {0};
    ray_t diffuse_ray;
    real_t cos_diffuse_angle;
    pix_t specular = {0};
    ray_t specular_ray = {0};
    real_t cos_specular_angle;
    patch_t PTRC nearest_patch = NULL;
    vec3_t nearest_patch_normal = {0};
    material_t PTRC mtl = NULL;
    pix_t *pixels[8];
    material_t const default_mtl = {
            .k = {
                    .a = {0.1, 0.1, 0.1},
                    .d = {1, 1, 1},
                    .s = {1, 1, 1}
            }
    };

    if (depth == max_depth) {
        pix->r = pix->g = pix->b = 0;
        return FALSE;
    }

    // visibility
    if (strcmp(method, "naive") == 0) {
        find_nearest_patch(ray, scene, &nearest_patch, &hit_point, &mtl, NULL);
    } else if (str_startswith(method, "bvh")) {
        find_nearest_patch_bvh(ray, args, &nearest_patch, &hit_point, &mtl,
                               NULL);
    } else {
        fprintf(stderr, "Method not supported: %s\n", method);
        return FALSE;
    }

    if (nearest_patch == NULL) {
        pix->r = pix->g = pix->b = 0;
        return FALSE;
    }

    nearest_patch_normal = nearest_patch->normal;
    if (vec3_dot(&ray->direction, &nearest_patch_normal) > 0) {
        vec3_scale(&nearest_patch_normal, -1);
    }

    if (mtl == NULL) {
        mtl = &default_mtl;
    }

    // ambient
    for (i = 0; i < 3; ++i) {
        ambient.d[i] = (uint8_t) (0xff * mtl->k.a.d[i]);
    }

    // diffuse
    diffuse_ray.origin = hit_point;
    for (i = 0; i < scene->num_lights; ++i) {
        light = scene->lights + i;
        vec3_minus(&diffuse_ray.direction, &light->origin, &hit_point);
        vec3_normalize(&diffuse_ray.direction);
        if (strcmp(method, "naive") == 0) {
            find_nearest_patch(&diffuse_ray, scene, &patch,
                               NULL, NULL, nearest_patch);
        } else if (str_startswith(method, "bvh")) {
            find_nearest_patch_bvh(&diffuse_ray, args, &patch,
                                   NULL, NULL, nearest_patch);
        } else {
            fprintf(stderr, "Method not supported: %s\n", method);
            return FALSE;
        }
        if (patch != NULL) {
            continue;
        }
        cos_diffuse_angle = vec3_cos_intersection_angle(&diffuse_ray.direction,
                                                        &nearest_patch_normal);
        if (cos_diffuse_angle < 0) {
            continue;
        }
        for (j = 0; j < 3; ++j) {
            tmp = diffuse.d[j]
                  + 0.8 * cos_diffuse_angle * mtl->k.d.d[j] * light->color.d[j];
            if (tmp < 0xff) {
                diffuse.d[j] = tmp;
            } else {
                diffuse.d[j] = 0xff;
            }
        }
    }

    // specular
    patch_reflect(nearest_patch, ray, &hit_point, &specular_ray);
    cos_specular_angle = -vec3_cos_intersection_angle(&ray->direction,
                                                      &nearest_patch_normal);
    if (cos_specular_angle < 0) {
        specular.r = specular.g = specular.b = 0;
    } else {
        fill_color(&specular, &specular_ray, scene, depth + 1, max_depth,
                   method, args);
        for (i = 0; i < 3; ++i) {
            specular.d[i] *= 0.8 * mtl->k.d.d[i] * cos_specular_angle;
        }
    }

    pixels[0] = &ambient;
    pixels[1] = &diffuse;
    pixels[2] = &specular;
    pix_accumulate(pixels, 3, pix);

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