//
// Created by quentin on 5/24/19.
//

#ifndef STARS_SIMULATOR_GALAXY_H
#define STARS_SIMULATOR_GALAXY_H

#include "star.h"
#include "vector.h"
#include "box.h"

typedef struct galaxy {
    int num_bodies;
    star **stars;
    box *b;
} galaxy;

double drand_f(double min, double max) ;

galaxy *create_and_init_galaxy(int num_bodies, box *b, double dt);

void reset_accelerations(galaxy *g);

void update_positions(galaxy *g, double dt);

void free_galaxy(galaxy *g);

void resize_galaxy(galaxy *g);

#endif //STARS_SIMULATOR_GALAXY_H
