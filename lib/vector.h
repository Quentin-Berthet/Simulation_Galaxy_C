//
// Created by quentin on 5/21/19.
//

#ifndef STARS_SIMULATOR_VECTOR_H
#define STARS_SIMULATOR_VECTOR_H
typedef struct vec {
    double x;
    double y;
} vec;

vec *new_vec(double x, double y);

vec *add_vec(const vec *const v1, const vec *const v2);

vec *sub_vec(const vec *const v1, const vec *const v2);

vec *mul_vec(double alpha, const vec *const v2);

double norm(vec *v1);

double distance(const vec *const v1, const vec *const v2);

void print_vec(const vec *const v);

void free_vec(vec *v);

#endif //STARS_SIMULATOR_VECTOR_H
