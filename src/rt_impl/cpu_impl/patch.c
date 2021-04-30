//
// Created by karl on 4/12/21.
//

#include <stdio.h>

#include <cblas.h>
#include <lapacke.h>

#include <patch.h>

bool_t patch_hit(patch_t PTRC patch, ray_t PTRC ray, real_t CPTR dist,
                 vec3_t CPTR hit_point) {
    size_t i;
    mat3_t base = {0};
    vec3_t u_v_t;

    lapack_int ipiv[9] = {0};

    // base.r[0] = lower.v[1] - lower.v[0]
    cblas_dcopy(3, &patch->vertices[1], 1, &base.r[0], 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &base.r[0], 1);

    // base.r[1] = lower.v[2] - lower.v[0]
    cblas_dcopy(3, &patch->vertices[2], 1, &base.r[1], 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &base.r[1], 1);

    // base.r[2] = - ray.direction
    cblas_daxpy(3, -1, &ray->direction, 1, &base.r[2], 1);

    // the ray is parallel to the patch
    if (LAPACKE_dgetrf(LAPACK_ROW_MAJOR, 3, 3, &base, 3, ipiv) != 0) {
        return FALSE;
    }

    // Solve `base * [u, v, t] = ray.origin - lower.v[0]` for `u, v, t`
    cblas_dcopy(3, &ray->origin, 1, &u_v_t, 1);
    cblas_daxpy(3, -1, &patch->vertices[0], 1, &u_v_t, 1);
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'T', 3, 1, &base, 3, ipiv, &u_v_t, 1);
    for (i = 0; i < 3; ++i) {
        if (u_v_t.d[i] < 1e-3) {
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
