//
// Created by quentin on 5/23/19.
//


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "vector.h"
#include "box.h"

#ifndef STARS_SIMULATOR_START_H
#define STARS_SIMULATOR_START_H
typedef struct star {
    vec *pos_t;
    vec *pos_t_dt;
    vec *acc;
    double mass;
} star;

star *new_star_vel(vec *pos, vec *vel, vec *acc, double mass, double dt);

void reset_acceleration(star *s);

void update_acceleration(star *s,  star * s2);

void update_position(star *s, double dt);

void print_star( star * s);

void free_star(star *s);

#endif //STARS_SIMULATOR_START_H
