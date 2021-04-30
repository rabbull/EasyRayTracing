//
// Created by 榨汁机 on 4/29/2021.
//

#include <stdlib.h>
#include <memory.h>

#include <vector.h>

vector_t *vector_new(size_t el_size, size_t capability) {
    vector_t *vec = malloc(sizeof(vector_t));
    if (capability == 0) {
        capability = 4;
    }
    vec->capability = capability;
    vec->el_size = el_size;
    vec->length = 0;
    vec->payload = calloc(capability, el_size);
    return vec;
}

void *vector_at(vector_t PTRC vector, size_t index) {
    if (index >= vector->length) {
        return NULL;
    }
    return ptr_offset(vector->payload, index * vector->el_size);
}

void vector_append(vector_t *vector, void *element) {
    if (vector->length == vector->capability) {
        vector->capability *= 2;
        vector->payload = realloc(vector->payload,
                                  vector->capability * vector->el_size);
    }
    memcpy(vector_at(vector, vector->length++), element, vector->el_size);
}

static void
do_vector_intersection_sort_by_weights(vector_t *self, real_t *weights,
                                       size_t l, size_t r, void *buf) {
    size_t i, j;
    size_t k;
    real_t w;

    for (i = 0; i < self->length - 1; ++i) {
        k = i;
        for (j = i + 1; j < self->length; ++j) {
            if (weights[j] < weights[k]) {
                k = j;
            }
        }
        memcpy(buf, vector_at(self, i), self->el_size);
        memcpy(vector_at(self, i), vector_at(self, k), self->el_size);
        memcpy(vector_at(self, k), buf, self->el_size);
        w = weights[i];
        weights[i] = weights[k];
        weights[k] = w;
    }
}

static void do_vector_sort_by_weights(vector_t *self, real_t *weights,
                                      size_t l, size_t r, void *buf) {
    size_t i, j;
    real_t w;

    if (r - l < 5) {
        do_vector_intersection_sort_by_weights(self, weights, l, r, buf);
        return;
    }

    i = 1;
    j = r - 1;
    while (i < j) {
        while (i < j && weights[i] <= weights[0]) {
            i += 1;
        }
        while (i < j && weights[0] < weights[j]) {
            j -= 1;
        }
        if (i >= j) {
            break;
        }

        w = weights[i];
        weights[i] = weights[j];
        weights[j] = w;
        memcpy(buf, vector_at(self, i), self->el_size);
        memcpy(vector_at(self, i), vector_at(self, j), self->el_size);
        memcpy(vector_at(self, j), buf, self->el_size);
    }
    j -= 1;
    w = weights[0];
    weights[0] = weights[j];
    weights[j] = w;
    memcpy(buf, vector_at(self, 0), self->el_size);
    memcpy(vector_at(self, 0), vector_at(self, j), self->el_size);
    memcpy(vector_at(self, j), buf, self->el_size);

    do_vector_sort_by_weights(self, weights, l, j, buf);
    do_vector_sort_by_weights(self, weights, j + 1, r, buf);
}

void vector_sort_by_weights(vector_t *self, real_t *weights) {
    byte_t *el;
    el = malloc(self->el_size);
    do_vector_sort_by_weights(self, weights, 0, self->length, el);
    free(el);
}

void vector_destroy(vector_t *vector) {
    free(vector->payload);
    free(vector);
}