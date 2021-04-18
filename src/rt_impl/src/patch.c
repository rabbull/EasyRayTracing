//
// Created by karl on 4/12/21.
//

#include <cblas.h>
#include <lapacke.h>

#include "include/patch.h"

bool_t hit(patch_t CPTR patch, ray_t CPTRC ray, real_t CPTR dist,
           vec3_t CPTR hit_point) {
    size_t i;
    real_t len_normal;
    mat3_t base = {0};
    vec3_t vec3, u_v_t;
    real_t *p, *q;

    lapack_int ipiv[9] = {0};

    // base.r[0] = p.vec3[1] - p.vec3[0]
    cblas_dcopy(3, &patch->vertices[1], 1, &base.r[0], 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &base.r[0], 1);

    // base.r[1] = p.vec3[2] - p.vec3[0]
    cblas_dcopy(3, &patch->vertices[2], 1, &base.r[1], 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &base.r[1], 1);

    // base.r[2] = - ray.direction
    cblas_daxpy(3, -1, &ray->direction, 1, &base.r[2], 1);

    // if normal vector of the patch is not set, it can be given by
    // `normal = base[0] * base[1]`, where `*` denotes a cross product
    if (cblas_dnrm2(3, &patch->normal, 1) == 0) {
        // use `vec3` to temporarily store non-unit normal vector
        p = (real_t *) &vec3;
        q = (real_t *) &base;

        // p[0] = base[0][1] * base[1][2] - base[0][2] * base[1][1];
        // p[1] = base[0][2] * base[1][0] - base[0][0] * base[1][2];
        // p[2] = base[0][0] * base[1][1] - base[0][1] * base[1][0];
        p[0] = q[1] * q[5] - q[2] * q[4];
        p[1] = q[2] * q[3] - q[0] * q[5];
        p[2] = q[0] * q[4] - q[1] * q[3];

        len_normal = cblas_dnrm2(3, p, 1);
        cblas_daxpy(3, 1. / len_normal, p, 1, &patch->normal, 1);
    }

    if (eq(cblas_ddot(3, &patch->normal, 1, &ray->direction, 1), 0)) {
        return OFF_TARGET;
    }

    // Solve `base * [u, vec3, t] = ray.origin - 2 * p.vec3[0]` for `u, vec3, t`

    // let `u_v_t` = `ray.origin - p.vec3[0]`
    cblas_dcopy(3, &ray->origin, 1, &u_v_t, 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &u_v_t, 1);

    LAPACKE_dgetrf(LAPACK_ROW_MAJOR, 3, 3, &base, 3, ipiv);
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'T', 3, 1, &base, 3, ipiv, &u_v_t, 1);
    for (i = 0; i < 3; ++i) {
        if (u_v_t.d[i] < 0) {
            return OFF_TARGET;
        }
    }
    if (u_v_t.d[0] + u_v_t.d[1] > 1) {
        return OFF_TARGET;
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

void color(pix_t CPTR pix, ray_t CPTR ray, size_t const num_patches,
           patch_t CPTRC patches) {
    size_t i;
    bool_t flag;

    for (i = 0; i < num_patches; ++i) {
    }
}
