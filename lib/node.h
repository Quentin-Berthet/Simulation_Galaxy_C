//
// Created by quentin on 6/1/19.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "star.h"
#include "vector.h"
#include "box.h"
#include "galaxy.h"

#define NB_CHILDREN 4

typedef struct node {
    struct node **children;
    box *b;
    star *s;
    star *super_s;
    bool is_empty;
} node;

bool is_leaf(const node *const n);

node *node_create(star *s, box *b);

void insert_star_in_node(node *n, star *s);

void remove_star(node *n);

void remove_super_star(node *n);

star *creat_super_star(star *s1, star *s2);

void free_node(node *n);

#endif //PROJECT_NODE_H
