//
// Created by 榨汁机 on 4/28/2021.
//

#ifndef RT_IMPL_BVH_H
#define RT_IMPL_BVH_H

#include <stdlib.h>

#include <scene.h>
#include <linalg.h>

typedef enum {
    BVH_SPLIT_MIDDLE,
    BVH_SPLIT_SAH
} bvh_split_method_t;

typedef union {
    struct {
        vec3_t lower;
        vec3_t upper;
    };
    real_t d[6];
} aabb_t;

typedef struct {
    patch_t *patch;
    material_t *mtl;

    vec3_t patch_center;
} bvh_payload_t;

typedef struct bvh_tree_node {
    struct bvh_tree_node *parent;
    struct bvh_tree_node *left;
    struct bvh_tree_node *right;

    size_t payload_slice_size;
    bvh_payload_t *payload_slice;

    aabb_t aabb;
} bvh_tree_node_t;

typedef struct {
    bvh_tree_node_t *root;

    size_t num_payloads;
    bvh_payload_t *payloads;
} bvh_tree_t;

bool_t aabb_hit(aabb_t PTRC aabb, ray_t PTRC ray);

bvh_tree_t *bvh_tree_construct(scene_t PTRC scene, bvh_split_method_t method);

bool_t bvh_tree_node_is_leaf(bvh_tree_node_t PTRC node);

void bvh_tree_destroy(bvh_tree_t *tree);

void print_bvh_tree_node(bvh_tree_node_t PTRC node, bvh_tree_t PTRC tree);

#endif //RT_IMPL_BVH_H
