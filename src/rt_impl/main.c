//
// Created by karl on 4/12/21.
//

#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <cblas.h>

#include "src/include/linalg.h"

int main(int argc, char **argv) {
    real_t A[4] = {1, 2, 3, 4};
    real_t B[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    real_t res[256] = {0};

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                2, 4, 2,
                1, A, 2, B, 2,
                0, res, 4);

    for (int i = 0; i < 256; ++i) {
        printf("%lf\n", res[i]);
    }
}
