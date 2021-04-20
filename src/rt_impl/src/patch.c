//
// Created by karl on 4/12/21.
//

#include <cblas.h>
#include <lapacke.h>
#include <stdio.h>

#include "include/patch.h"

bool_t hit(patch_t CPTRC patch, ray_t CPTRC ray, real_t CPTR dist,
           vec3_t CPTR hit_point) {
    size_t i;
    mat3_t base = {0};
    vec3_t u_v_t;

    lapack_int ipiv[9] = {0};

    // base.r[0] = p.v[1] - p.v[0]
    cblas_dcopy(3, &patch->vertices[1], 1, &base.r[0], 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &base.r[0], 1);

    // base.r[1] = p.v[2] - p.v[0]
    cblas_dcopy(3, &patch->vertices[2], 1, &base.r[1], 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &base.r[1], 1);

    // base.r[2] = - ray.direction
    cblas_daxpy(3, -1, &ray->direction, 1, &base.r[2], 1);

    // the ray is parallel to the patch
    if (LAPACKE_dgetrf(LAPACK_ROW_MAJOR, 3, 3, &base, 3, ipiv) != 0) {
        return FALSE;
    }

    // Solve `base * [u, v, t] = ray.origin - p.v[0]` for `u, v, t`
    cblas_dcopy(3, &ray->origin, 1, &u_v_t, 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &u_v_t, 1);
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'T', 3, 1, &base, 3, ipiv, &u_v_t, 1);
    for (i = 0; i < 3; ++i) {
        if (u_v_t.d[i] < 0) {
            return FALSE;
        }
    }
    if (u_v_t.d[0] + u_v_t.d[1] > 1) {
        return FALSE;
    }

    if (hit_point != NULL) {
        cblas_dcopy(3, &ray->origin, 1, hit_point, 1);
        cblas_daxpy(3, u_v_t.d[2], &ray->direction, 1, hit_point, 1);
    }

    *dist = u_v_t.d[2];
    return TRUE;
}

void reflect(patch_t CPTRC patch, ray_t CPTRC ray, vec3_t CPTRC hit_point,
             ray_t CPTR reflection) {
    vec3_t patch_normal_unit = {0};
    vec3_t ray_direction_unit = {0};

    cblas_dcopy(3, hit_point, 1, &reflection->origin, 1);
    cblas_daxpy(3, 1. / cblas_dnrm2(3, &ray->direction, 1), &ray->direction, 1,
                &ray_direction_unit, 1);
    cblas_dcopy(3, &ray->direction, 1, &reflection->direction, 1);
    cblas_daxpy(3, 2. *
                   cblas_ddot(3, &ray_direction_unit, 1, &patch_normal_unit, 1),
                &patch_normal_unit, 1, &reflection->direction, 1);
}

void print_patch(patch_t CPTRC patch, char PTRC name, char PTRC prefix) {
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
