//
// Created by 榨汁机 on 5/2/2021.
//

#include <stdio.h>

#include <ray.h>

void ray_print(ray_t CPTRC ray, char CPTRC name) {
    printf("%s:\n", name);
    print_vec3(&ray->origin, "\torigin");
    print_vec3(&ray->direction, "\tdirection");
}
