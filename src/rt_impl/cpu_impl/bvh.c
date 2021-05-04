//
// Created by 榨汁机 on 4/28/2021.
//

#include <stdio.h>

#include <cblas.h>

#include <bvh.h>

bool_t aabb_hit(aabb_t CPTRC aabb, ray_t CPTRC ray) {
    size_t axis;
    real_t CPTR lower = (real_t *) &aabb->lower;
    real_t CPTR upper = (real_t *) &aabb->upper;
    real_t t[2][3] = {0};
    real_t CPTR direction = (real_t *) &ray->direction;
    real_t CPTR origin = (real_t *) &ray->origin;

    for (axis = 0; axis < 3; ++axis) {
        if (real_abs(direction[axis]) < real_esp) {
            if (origin[axis] > upper[axis] || origin[axis] < lower[axis]) {
                return FALSE;
            }
        } else {
            t[direction[axis] < 0][axis] =
                    (lower[axis] - origin[axis]) / direction[axis];
            t[direction[axis] > 0][axis] =
                    (upper[axis] - origin[axis]) / direction[axis];
        }
    }
    for (axis = 1; axis < 3; ++axis) {
        if (t[0][0] < t[0][axis]) {
            t[0][0] = t[0][axis];
        }
        if (t[1][0] > t[1][axis]) {
            t[1][0] = t[1][axis];
        }
    }
    return t[0][0] < t[1][0];
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

static void aabb_extend_for(aabb_t CPTR aabb, patch_t CPTRC patch) {
    size_t i, axis;
    real_t tmp;

    for (i = 0; i < 3; ++i) {
        for (axis = 0; axis < 3; ++axis) {
            tmp = patch->vertices[i].d[axis];
            if (tmp < aabb->lower.d[axis]) {
                aabb->lower.d[axis] = tmp;
            }
            if (tmp > aabb->upper.d[axis]) {
                aabb->upper.d[axis] = tmp;
            }
        }
    }
}

static bool_t bvh_tree_construct_should_terminate(bvh_tree_node_t CPTRC node) {
    return node->payload_slice_size < 16;
}

static void do_bvh_tree_construct(bvh_tree_node_t *node,
                                  bvh_split_method_t const method) {
    size_t i;
    size_t l, r;
    size_t plane_axis;
    real_t plane_pos;
    size_t longest_edge;
    real_t edge_length;
    real_t max_edge_length = -real_inf;
    bvh_payload_t tmp_payload;

    if (bvh_tree_construct_should_terminate(node)) {
        return;
    }

    node->left = calloc(2, sizeof(bvh_tree_node_t));
    node->right = node->left + 1;

    for (plane_axis = 0; plane_axis < 3; ++plane_axis) {
        edge_length =
                node->aabb.upper.d[plane_axis] - node->aabb.lower.d[plane_axis];
        if (edge_length > max_edge_length) {
            max_edge_length = edge_length;
            longest_edge = plane_axis;
        }
    }
    plane_axis = longest_edge;

    if (method == BVH_SPLIT_MIDDLE) {
        plane_pos = (
                            node->aabb.lower.d[plane_axis] +
                            node->aabb.upper.d[plane_axis]
                    ) / 2.;
    } else if (method == BVH_SPLIT_SAH) {
        plane_pos = 0;
    } else {
        return;
    }

    l = 0;
    r = node->payload_slice_size - 1;
    for (i = 0; i < 3; ++i) {
        node->left->aabb.lower.d[i] = real_inf;
        node->left->aabb.upper.d[i] = -real_inf;
        node->right->aabb.lower.d[i] = real_inf;
        node->right->aabb.upper.d[i] = -real_inf;
    }
    while (TRUE) {
        while (node->payload_slice[l].patch_center.d[plane_axis] <= plane_pos
               && l < r) {
            aabb_extend_for(&node->left->aabb, node->payload_slice[l].patch);
            l += 1;
        }
        while (node->payload_slice[r].patch_center.d[plane_axis] > plane_pos
               && l < r) {
            aabb_extend_for(&node->right->aabb, node->payload_slice[r].patch);
            r -= 1;
        }
        if (l == r) {
            break;
        }
        tmp_payload = node->payload_slice[l];
        node->payload_slice[l] = node->payload_slice[r];
        node->payload_slice[r] = tmp_payload;
    }
    if (l == 0 || r == node->payload_slice_size - 1) {
        free(node->left);
        node->left = node->right = NULL;
        return;
    }

    node->left->payload_slice = node->payload_slice;
    node->left->payload_slice_size = l;
    node->right->payload_slice = node->payload_slice + l;
    node->right->payload_slice_size = node->payload_slice_size - l;

    node = node->left;
#pragma omp parallel for shared(node, method) private(i)
    for (i = 0; i < 2; ++i) {
        do_bvh_tree_construct(node + i, method);
    }
}

bvh_tree_t *bvh_tree_construct(scene_t CPTRC scene,
                               bvh_split_method_t const method) {
    size_t i;

    bvh_payload_t *payload;
    bvh_tree_t *tree = malloc(sizeof(bvh_tree_t));

    tree->root = malloc(sizeof(bvh_tree_node_t));
    tree->num_payloads = scene->num_patches;
    tree->payloads = calloc(tree->num_payloads, sizeof(bvh_payload_t));
    for (i = 0; i < 3; ++i) {
        tree->root->aabb.lower.d[i] = real_inf;
        tree->root->aabb.upper.d[i] = -real_inf;
    }
    for (i = 0; i < scene->num_patches; ++i) {
        payload = tree->payloads + i;
        payload->patch = scene->patches + i;
        payload->mtl = scene->patch_material[i];
        vec3_avg(&payload->patch_center, payload->patch->vertices, 3);
        aabb_extend_for(&tree->root->aabb, payload->patch);
    }

    tree->root->parent = NULL;
    tree->root->left = tree->root->right = NULL;
    tree->root->payload_slice = tree->payloads;
    tree->root->payload_slice_size = tree->num_payloads;
    do_bvh_tree_construct(tree->root, method);

    return tree;
}

bool_t bvh_tree_node_is_leaf(bvh_tree_node_t CPTRC node) {
    return node->left == NULL && node->right == NULL;
}

static void bvh_tree_node_destroy_children(bvh_tree_node_t CPTR node) {
    if (bvh_tree_node_is_leaf(node)) {
        return;
    }
    bvh_tree_node_destroy_children(node->left);
    bvh_tree_node_destroy_children(node->right);
    free(node->left);
}

static void bvh_tree_node_destroy(bvh_tree_node_t CPTR node) {
    bvh_tree_node_destroy_children(node);
    free(node);
}

void bvh_tree_destroy(bvh_tree_t CPTR tree) {
    free(tree->payloads);
    bvh_tree_node_destroy(tree->root);
    free(tree);
}

void print_bvh_tree_node(const bvh_tree_node_t *node, const bvh_tree_t *tree) {
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
