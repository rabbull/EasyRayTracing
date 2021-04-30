//
// Created by 榨汁机 on 4/29/2021.
//

#ifndef RT_IMPL_QUEUE_H
#define RT_IMPL_QUEUE_H

#include <stdlib.h>
#include <utils.h>

typedef struct {
    void *payload;
    size_t capability;
    size_t el_size;
    size_t length;
} vector_t;

vector_t *vector_new(size_t el_size, size_t capability);

void *vector_at(vector_t PTRC vector, size_t index);

void vector_append(vector_t *vector, void *element);

typedef struct {
    void* args;
    bool_t (*cmp)(void *p, void *q, void *args);
    void (*swap)(void *p, void *q, void *args);
} sort_helper_t;

void vector_sort_by_weights(vector_t *self, real_t *weights);

void vector_destroy(vector_t *vector);

#endif //RT_IMPL_QUEUE_H
