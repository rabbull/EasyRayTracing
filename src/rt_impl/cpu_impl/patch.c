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
    real_t *normal = (real_t *) &patch->normal;
    real_t *o_origin = (real_t *) &reflection->origin;
    real_t *o_dir = (real_t *) &reflection->direction;
    real_t *i_dir = (real_t *) &ray->direction;

    vec3_t normal_u = {0};

    cblas_daxpy(3, 1 / cblas_dnrm2(3, normal, 1), normal, 1, &normal_u, 1);

    cblas_dcopy(3, hit_point, 1, o_origin, 1);
    cblas_dcopy(3, i_dir, 1, o_dir, 1);
    cblas_daxpy(3, 2 * cblas_ddot(3, i_dir, 1, &normal_u, 1),
                &normal_u, 1, o_dir, 1);
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
