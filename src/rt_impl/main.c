//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <cblas.h>

#include "src/include/linalg.h"
#include "src/include/alg.h"

int main(int argc, char **argv) {
    mat3_t mat = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    mat3_transpose(&mat, NULL);
    print_mat3(&mat, "mat");
}
