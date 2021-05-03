//
// Created by karl on 4/12/21.
//

#include <stdio.h>

#include <cblas.h>
#include <lapacke.h>

#include <patch.h>

static bool_t moller_trumbore_intersection_algorithm(patch_t CPTRC patch,
                                                     ray_t CPTRC ray,
                                                     real_t CPTR dist) {
    vec3_t e[2];
    vec3_t t, p, q;
    real_t u, v, d;
    real_t det, inv_det;

    vec3_minus(&e[0], &patch->vertices[1], &patch->vertices[0]);
    vec3_minus(&e[1], &patch->vertices[2], &patch->vertices[0]);
    vec3_cross(&p, &ray->direction, &e[1]);
    det = vec3_dot(&e[0], &p);

    if (real_abs(det) < real_esp) {
        return FALSE;
    }
    inv_det = 1. / det;
    vec3_minus(&t, &ray->origin, &patch->vertices[0]);
    u = vec3_dot(&t, &p) * inv_det;
    if (u < 0. || u > 1.) {
        return FALSE;
    }
    vec3_cross(&q, &t, &e[0]);
    v = vec3_dot(&ray->direction, &q) * inv_det;
    if (v < 0. || u + v > 1.) {
        return FALSE;
    }
    d = vec3_dot(&e[1], &q) * inv_det;
    if (d < real_esp) {
        return FALSE;
    }
    *dist = d;
    return TRUE;
}

bool_t patch_hit(patch_t PTRC patch, ray_t PTRC ray, real_t CPTR dist,
                 vec3_t CPTR hit_point) {
    if (moller_trumbore_intersection_algorithm(patch, ray, dist)) {
        vec3_copy(hit_point, &ray->origin);
        vec3_axpy(*dist, &ray->direction, hit_point);
        return TRUE;
    }
    return FALSE;
}

void patch_reflect(patch_t CPTRC patch, ray_t CPTRC ray, vec3_t CPTRC hit_point,
                   ray_t CPTR reflection) {
    vec3_t u_normal = {0};
    real_t alpha;

    vec3_copy(&u_normal, &patch->normal);
    vec3_scale(&u_normal, 1. / vec3_norm2(&u_normal));

    vec3_copy(&reflection->origin, hit_point);

    vec3_copy(&reflection->direction, &ray->direction);
    alpha = -2 * vec3_dot(&ray->direction, &u_normal);
    vec3_axpy(alpha, &u_normal, &reflection->direction);
}

void patch_print(const patch_t *patch, char PTRC name, char PTRC prefix) {
    size_t i;
    char vec_name[32];

    if (name == NULL) {
        name = "patch";
    }
    if (prefix == NULL) {
        prefix = "";
    }

    printf("%s%s = \n", prefix, name);
    sprintf(vec_name, "%s\tnormal", prefix);
    print_vec3(&patch->normal, vec_name);
    printf("%s\tvertices =\n", prefix);
    for (i = 0; i < 3; ++i) {
        sprintf(vec_name, "%s\t\t[%zu]", prefix, i);
        print_vec3(patch->vertices + i, vec_name);
    }
}
