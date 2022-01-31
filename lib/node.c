//
// Created by quentin on 6/1/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "node.h"

bool is_leaf(const node *const n) {
    for (int i = 0; i < NB_CHILDREN; ++i) {
        if (n->children[i] != NULL) {
            return false;
        }
    }
    return true;
}

void insert_star_in_node(node *n, star *star) {
    assert(n != NULL);
    if (star != NULL) {
        n->s = star;
        n->is_empty = false;
    }
}

void insert_super_star(node *n, star *super_star) {
    assert(n != NULL);
    if (super_star != NULL) {
        n->super_s = super_star;
        n->is_empty = false;
    }
}

node *node_create(star *s, box *b) {
    node *new_node = calloc(1, sizeof(node));
    new_node->b = b;
    new_node->s = s;
    new_node->super_s = NULL;
    new_node->is_empty = s == NULL;
    new_node->children = calloc(NB_CHILDREN, sizeof(node *));
    return new_node;
}

void remove_star(node *n) {
    assert(n != NULL);
    if (n->s != NULL) {
        n->s = NULL;
    } else {
        return;
    }
}

void remove_super_star(node *n) {
    assert(n != NULL);
    if (n->super_s != NULL) {
        n->super_s = NULL;
    }
}

star *creat_super_star(star *s1, star *s2) {
    double mass_tot = s1->mass + s2->mass;
    double m1 = s1->mass;
    double m2 = s2->mass;
    /*
     *       (m1*r1) + (m2*r2)
     * pos = ------------------
     *           (m1 + m2)
     */
    vec *d1_mult_m1 = mul_vec(m1, s1->pos_t);
    vec *d2_mult_m2 = mul_vec(m2, s2->pos_t);
    vec *num_pos = add_vec(d1_mult_m1, d2_mult_m2);
    double den_pos = 1 / mass_tot;
    vec *pos_t = mul_vec(den_pos, num_pos);
    vec *vel = new_vec(0.0, 0.0);
    star *super_star = new_star_vel(pos_t, vel, new_vec(0.0, 0.0), mass_tot, 1e10);

    //liberation memoire
    free_vec(vel);
    free_vec(d1_mult_m1);
    free_vec(d2_mult_m2);
    free_vec(num_pos);
    return super_star;
}


void free_node(node *n) {
    assert(n != NULL);
    for (int i = 0; i < NB_CHILDREN; ++i) {
        if (n->children[i] != NULL) {
            free_node(n->children[i]);  //libere chaque element d'enfant
        }
    }
    free(n->children);  //libere enfant

    if (n->super_s != NULL) {
        free_star(n->super_s);  //libere super_star
    }

    if (n->b != NULL) {
        free_box(n->b);     //libere box
    }
    n->is_empty = true;
    free(n);    //libere node
}
