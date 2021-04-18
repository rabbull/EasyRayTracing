//
// Created by karl on 4/12/21.
//

#ifndef RT_IMPL_UTILS_H
#define RT_IMPL_UTILS_H

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef double real_t;

typedef struct {
    real_t d[3];
} vec3_t;

typedef struct {
    vec3_t r[3];
} mat3_t;

#define vec_at(vec, index) ((vec).d[index])
#define mat_at(mat, row, col) ((mat).r[row].d[col])

#define PTRC const*
#define CPTR *const
#define CPTRC const*const

void print_vec3(vec3_t PTRC vec, char PTRC name);

void print_mat3(mat3_t PTRC mat, char PTRC name);

bool_t eq(real_t p, real_t q);

#endif //RT_IMPL_UTILS_H
