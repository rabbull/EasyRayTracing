//
// Created by 榨汁机 on 4/28/2021.
//

#include <stdio.h>

#include <cblas.h>

#include <kdtree.h>

bool_t aabb_hit(aabb_t CPTRC aabb, ray_t CPTRC ray, real_t CPTR distance) {
    size_t axis;
    real_t CPTR lower = (real_t *) &aabb->lower;
    real_t CPTR upper = (real_t *) &aabb->upper;
    real_t t;
    real_t CPTR direction = (real_t *) &ray->direction;
    real_t CPTR origin = (real_t *) &ray->origin;

    *distance = real_inf;
    for (axis = 0; axis < 3; ++axis) {
        t = (lower[axis] - origin[axis]) / direction[axis];
        if (t >= real_esp && t < *distance) {
            *distance = t;
        }
        t = (upper[axis] - origin[axis]) / direction[axis];
        if (t >= real_esp && t < *distance) {
            *distance = t;
        }
    }
    return *distance < real_inf;
}

static bool_t aabb_contains_patch(aabb_t CPTRC aabb, patch_t CPTRC patch) {
    size_t i, j;
    real_t r;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            r = patch->vertices[i].d[j];
            if (r < aabb->lower.d[j] || r > aabb->upper.d[j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

static real_t aabb_volume(aabb_t CPTRC aabb) {
    real_t tmp[3] = {0};
    cblas_dcopy(3, &aabb->upper, 1, tmp, 1);
    cblas_daxpy(3, -1, &aabb->lower, 1, tmp, 1);
    return cblas_dnrm2(3, tmp, 1);
}

static bool_t kdtree_should_terminate(kdtree_node_t CPTRC node) {
    return node->payload_slice_size < 5;
}

static void do_kdtree_construct(kdtree_node_t *node) {
    size_t i, j;
    size_t l, r;
    size_t plane_axis;
    real_t plane_pos;
    size_t longest_edge;
    real_t edge_length;
    real_t max_edge_length = -real_inf;
    kdtree_payload_t payload;
    vec3_t aabb_q;

    if (kdtree_should_terminate(node)) {
        return;
    }
    node->children = calloc(2, sizeof(kdtree_node_t));

    // choose the longest edge to divide
    for (plane_axis = 0; plane_axis < 3; ++plane_axis) {
        edge_length =
                node->aabb.upper.d[plane_axis] - node->aabb.lower.d[plane_axis];
        if (edge_length > max_edge_length) {
            max_edge_length = edge_length;
            longest_edge = plane_axis;
        }
    }
    plane_axis = longest_edge;

    node->children[0].parent = node->children[1].parent = node;
    node->children[0].children = node->children[1].children = NULL;
    node->children[0].aabb = node->children[1].aabb = node->aabb;
    plane_pos = (
                        node->aabb.lower.d[plane_axis] +
                        node->aabb.upper.d[plane_axis]
                ) / 2.;
    node->children[0].aabb.upper.d[plane_axis] = plane_pos;
    node->children[1].aabb.lower.d[plane_axis] = plane_pos;

    l = 0;
    r = node->payload_slice_size - 1;
    aabb_q = node->children[0].aabb.upper;
    while (TRUE) {
        while (l < r && !aabb_contains_patch(
                &node->children[1].aabb, node->payload_slice[l].patch
        )) {
            payload = node->payload_slice[l];
            for (i = 0; i < 3; ++i) {
                for (j = 0; j < 3; ++j) {
                    if (payload.patch->vertices[i].d[j] > aabb_q.d[j]) {
                        aabb_q.d[j] = payload.patch->vertices[i].d[j];
                    }
                }
            }
            l += 1;
        }
        while (l < r && aabb_contains_patch(
                &node->children[1].aabb, node->payload_slice[r].patch
        )) {
            r -= 1;
        }
        if (l == r) {
            break;
        }
        payload = node->payload_slice[l];
        node->payload_slice[l] = node->payload_slice[r];
        node->payload_slice[r] = payload;
    }
    node->children[0].payload_slice = node->payload_slice;
    node->children[0].payload_slice_size = l;
    node->children[1].payload_slice = node->payload_slice + l;
    node->children[1].payload_slice_size = node->payload_slice_size - l;
//    print_kdtree_node(node, the_tree);
    do_kdtree_construct(node->children);
    do_kdtree_construct(node->children + 1);
    node->children[0].aabb.upper = aabb_q;
}

kdtree_t *kdtree_construct(scene_t CPTRC scene) {
    size_t i, j, axis;
    real_t r;

    kdtree_t *tree = malloc(sizeof(kdtree_t));

    // initialize tree
    tree->k = 2;
    tree->root = malloc(sizeof(kdtree_node_t));
    tree->num_payloads = scene->num_patches;
    tree->payloads = calloc(tree->num_payloads, sizeof(kdtree_payload_t));
    for (i = 0; i < 3; ++i) {
        tree->root->aabb.lower.d[i] = real_inf;
        tree->root->aabb.upper.d[i] = -real_inf;
    }
    for (i = 0; i < scene->num_patches; ++i) {
        tree->payloads[i].patch = scene->patches + i;
        tree->payloads[i].mtl = scene->patch_material[i];
        for (j = 0; j < 3; ++j) {
            for (axis = 0; axis < 3; ++axis) {
                r = scene->patches[i].vertices[j].d[axis];
                if (r < tree->root->aabb.lower.d[axis]) {
                    tree->root->aabb.lower.d[axis] = r;
                }
                if (r > tree->root->aabb.upper.d[axis]) {
                    tree->root->aabb.upper.d[axis] = r;
                }
            }
        }
    }

    tree->root->parent = NULL;
    tree->root->children = NULL;
    tree->root->payload_slice = tree->payloads;
    tree->root->payload_slice_size = tree->num_payloads;

    do_kdtree_construct(tree->root);

    return tree;
}

bool_t kdtree_node_is_leaf(kdtree_node_t CPTRC node) {
    return node->children == NULL;
}

static void kdtree_node_destroy_children(kdtree_node_t CPTR node, size_t k) {
    size_t i;
    if (kdtree_node_is_leaf(node)) {
        return;
    }
    for (i = 0; i < k; ++i) {
        kdtree_node_destroy_children(node->children + i, k);
    }
    free(node->children);
}

void kdtree_node_destroy(kdtree_node_t CPTR node, size_t k) {
    kdtree_node_destroy_children(node, k);
    free(node);
}

void kdtree_destroy(kdtree_t CPTR tree) {
    free(tree->payloads);
    kdtree_node_destroy(tree->root, tree->k);
    free(tree);
}

void print_kdtree_node(kdtree_node_t CPTRC node, kdtree_t CPTRC tree) {
    size_t from, to;
    from = node->payload_slice - tree->payloads;
    to = from + node->payload_slice_size;

    printf("node:\n");
    printf("\taabb:\n");
    print_vec3(&node->aabb.lower, "\t\tlower");
    print_vec3(&node->aabb.upper, "\t\tupper");
    printf("\tslice:\n");
    printf("\t\tfrom: %zu\n", from);
    printf("\t\tto:   %zu\n", to);
}
