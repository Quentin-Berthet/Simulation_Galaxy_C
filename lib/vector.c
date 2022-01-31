//
// Created by quentin on 5/21/19.
//

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"


vec *new_vec(double x, double y) {
    vec *new_vec = calloc(1, sizeof(vec));
    new_vec->x = x;
    new_vec->y = y;
    return new_vec;
}

vec *add_vec(const vec *const v1, const vec *const v2) {
    assert(v1 != NULL);
    assert(v2 != NULL);
    vec *vec_res = calloc(1, sizeof(vec));
    vec_res->x = v1->x + v2->x;
    vec_res->y = v1->y + v2->y;
    return vec_res;
}

vec *sub_vec(const vec *const v1, const vec *const v2) {
    assert(v1 != NULL);
    assert(v2 != NULL);
    vec *vec_res = calloc(1, sizeof(vec));
    vec_res->x = v1->x - v2->x;
    vec_res->y = v1->y - v2->y;
    return vec_res;
}

vec *mul_vec(double alpha, const vec *const v1) {
    assert(v1 != NULL);
    vec *vec_res = calloc(1, sizeof(vec));
    vec_res->x = v1->x * alpha;
    vec_res->y = v1->y * alpha;
    return vec_res;
}

double norm(vec *v1) {
    assert(v1 != NULL);
    double sum = (v1->x * v1->x) + (v1->y * v1->y);
    double norm = pow(sum, 0.5);
    return norm;
}

double distance(const vec *const v1, const vec *const v2) {
    assert(v1 != NULL);
    assert(v2 != NULL);
    double diff_x_2 = (v1->x - v2->x) * (v1->x - v2->x);
    double diff_y_2 = (v1->y - v2->y) * (v1->y - v2->y);
    double dist = pow(diff_x_2 + diff_y_2, 0.5);
    return dist;
}

void print_vec(const vec *const v) {
    printf("(%g; %g)", v->x, v->y);
}

void free_vec(vec *v) {
    free(v);
}