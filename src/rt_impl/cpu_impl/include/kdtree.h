//
// Created by 榨汁机 on 4/28/2021.
//

#ifndef RT_IMPL_KDTREE_H
#define RT_IMPL_KDTREE_H

#include <stdlib.h>

#include <scene.h>
#include <linalg.h>

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
} kdtree_payload_t;

typedef struct kdtree_node {
    struct kdtree_node *parent;
    struct kdtree_node *children;

    size_t payload_slice_size;
    kdtree_payload_t *payload_slice;

    aabb_t aabb;
} kdtree_node_t;

typedef struct {
    size_t k;
    kdtree_node_t *root;
    size_t num_payloads;
    kdtree_payload_t *payloads;
} kdtree_t;

bool_t aabb_hit(aabb_t PTRC aabb, ray_t PTRC ray, real_t *distance);

bool_t kdtree_node_is_leaf(const kdtree_node_t *const node);

void print_kdtree_node(kdtree_node_t PTRC node, kdtree_t PTRC tree);

kdtree_t *kdtree_construct(scene_t PTRC scene);

void kdtree_destroy(kdtree_t *tree);

#endif //RT_IMPL_KDTREE_H
