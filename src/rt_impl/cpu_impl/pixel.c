//
// Created by 榨汁机 on 4/21/2021.
//

#include <stdlib.h>
#include <stdio.h>

#include <cblas.h>

#include <linalg.h>
#include <pixel.h>

void pix_print(pix_t const *pixel, char const *name) {
    size_t i;

    printf("%s = [", name);
    for (i = 0; i < 4; ++i) {
        printf(&", %u"[(i == 0) << 1], pixel->d[i]);
    }
    printf("]\n");
}

static void avg(pix_t CPTR CPTR pixels, size_t const n, pix_t CPTR output) {
    size_t i, j;
    vec3_t accumulated = {0};
    for (i = 0; i < n; ++i) {
        for (j = 0; j < 3; ++j) {
            accumulated.d[j] += pixels[i]->d[j];
        }
    }
    vec3_scale(&accumulated, 1. / n);
    for (i = 0; i < 3; ++i) {
        output->d[i] = (uint8_t) accumulated.d[i];
    }
}

void blend(pix_t CPTR CPTR pixels, real_t CPTRC weights, size_t const n,
           pix_t CPTR output) {
    size_t i, j;
    real_t sum = 0;
    vec3_t accumulated = {0};

    if (weights == NULL) {
        avg(pixels, n, output);
        return;
    }

    for (i = 0; i < n; ++i) {
        sum += weights[i];
    }

    for (i = 0; i < n; ++i) {
        for (j = 0; j < 3; ++j) {
            accumulated.d[j] += pixels[i]->d[j] * weights[i];
        }
    }
    cblas_dscal(3, 1 / sum, &accumulated, 1);
    for (i = 0; i < 3; ++i) {
        output->d[i] = (uint8_t) accumulated.d[i];
    }
}
