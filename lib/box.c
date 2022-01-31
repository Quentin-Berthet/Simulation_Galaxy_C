//
// Created by quentin on 5/21/19.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "box.h"

box *new_box(double x0, double y0, double w, double h) {
    box *new_box = calloc(1, sizeof(box));
    new_box->x0 = x0;
    new_box->y0 = y0;
    new_box->w = w;
    new_box->h = h;
    return new_box;
}

box **divide_in_four(box *b) {
    box **divided_box = calloc(4, sizeof(box *));

    //box_0
    divided_box[0] = new_box(b->x0, b->y0, b->w / 2, b->h / 2);

    //box_1
    divided_box[1] = new_box((b->x0 + b->w) / 2, b->y0, b->w/2, b->h / 2);

    //box_2
    divided_box[2] = new_box(b->x0, (b->y0 + b->h) / 2, b->w / 2, b->h / 2);

    //box_3
    divided_box[3] = new_box((b->x0 + b->w) / 2, (b->y0 + b->h) / 2, b->w / 2, b->h / 2);


    return divided_box;
}

bool is_inside(box *b, vec *v) {
    if (v->x >= b->x0 && v->x <= (b->x0 + b->w) && v->y >= b->y0 && v->y <= (b->y0 + b->h)) {
        return true;
    }
    return false;
}

double compute_length(box *b) {
    double dist_0 = b->w;
    double dist_1 = b->h;
    if (dist_0 >= dist_1) {
        return dist_0;
    } else {
        return dist_1;
    }
}


void free_box(box *b) {
    free(b);
}

