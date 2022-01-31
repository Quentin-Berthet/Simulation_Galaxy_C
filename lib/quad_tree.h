//
// Created by quentin on 5/24/19.
//

#ifndef STARS_SIMULATOR_QUAD_TREE_H
#define STARS_SIMULATOR_QUAD_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "star.h"
#include "vector.h"
#include "box.h"
#include "galaxy.h"
#include "node.h"


typedef struct quad_tree {
    node *root;
} quad_tree;

quad_tree *create_quad_tree_from_galaxy(galaxy *g);

void free_quad_tree(quad_tree *t);

void update_acceleration_from_node(const node *const n, star *s, double theta);

void update_accelerations_of_all_stars(quad_tree *t, galaxy *g, double theta);

void free_quad_tree(quad_tree *t);
#endif //STARS_SIMULATOR_QUAD_TREE_H
