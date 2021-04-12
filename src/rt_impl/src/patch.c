//
// Created by karl on 4/12/21.
//

#include <mkl.h>
#include <mkl_lapacke.h>

#include "patch.h"
#include "ray.h"

int hit(patch_t CPTRC patch, ray_t CPTRC ray, vec3_t CPTR hit_point,
        int CPTR reflected, ray_t CPTR reflection,
        int CPTR transparent, ray_t CPTR transparency) {
    mat3_t base = {0};
    int ipiv[9] = {0};
    vec3_t normal, T;

    // base.r[0] = p.v[1] - p.v[0]
    cblas_dcopy(3, &patch->vertices[0], 1, &base.r[0], 1);
    cblas_daxpy(3, -1, &patch->vertices[1], 1, &base.r[0], 1);

    // base.r[1] = p.v[2] - p.v[0]
    cblas_dcopy(3, &patch->vertices[0], 1, &base.r[1], 1);
    cblas_daxpy(3, -1, &patch->vertices[2], 1, &base.r[1], 1);

    // base.r[2] = - ray.direction
    cblas_daxpy(3, -1, &ray->direction, 1, &base.r[2], 1);

    normal.d[0] = base.r[0].d[1] * base.r[1].d[2] - base.r[0].d[2] * base.r[1].d[1];
    normal.d[1] = base.r[0].d[2] * base.r[1].d[0] - base.r[0].d[0] * base.r[1].d[2];
    normal.d[2] = base.r[0].d[0] * base.r[1].d[1] - base.r[0].d[1] * base.r[1].d[0];

    if (eq(cblas_ddot(3, normal.d, 1, ray->direction.d, 1), 0)) {
        return 0;
    }

    // solve `[base:ray.direction] * [_, _, t] = ray.origin` for `t`
    // where the ray should intersect with the plane where the patch
    // is located at the point `ray.origin + ray.direction * t`
    LAPACKE_dgetrf(LAPACK_ROW_MAJOR, 3, 3, &base, 3, ipiv);
    cblas_dcopy(3, &ray->origin, 1, &T, 1);
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', 3, 1, &base, 3, ipiv, &T, 1);

    cblas_dcopy(3, &ray->origin, 1, hit_point, 1);
    cblas_daxpy(3, T.d[2], &ray->direction, 1, hit_point, 1);

    return 1;
}
