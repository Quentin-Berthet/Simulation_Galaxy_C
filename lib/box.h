//
// Created by quentin on 5/21/19.
//

#ifndef STARS_SIMULATOR_BOX_H
#define STARS_SIMULATOR_BOX_H

#include "vector.h"
#include <stdbool.h>

typedef struct box {
    double x0;
    double y0;
    double w;
    double h;

} box;

box *new_box(double x0, double y0, double w, double h);

box **divide_in_four(box *b);

bool is_inside(box *b, vec *v);

double compute_length(box *b);

void free_box(box *b);

#endif //STARS_SIMULATOR_BOX_H
