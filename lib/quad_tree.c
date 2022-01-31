#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "quad_tree.h"


void insert_star(node *n, star *s) {
    if (n != NULL && s != NULL && is_inside(n->b, s->pos_t)) {
        if (is_leaf(n)) {
            if (n->is_empty) {
                insert_star_in_node(n, s);
            } else {
                box **new_box = divide_in_four(n->b);
                for (int k = 0; k < NB_CHILDREN; ++k) {
                    n->children[k] = node_create(NULL, new_box[k]);
                }
                free(new_box);
                for (int i = 0; i < NB_CHILDREN; ++i) {
                    insert_star(n->children[i], n->s);
                }
                for (int j = 0; j < NB_CHILDREN; ++j) {
                    insert_star(n->children[j], s);
                }
                n->super_s = creat_super_star(n->s, s);
                remove_star(n);
            }
        } else {

            /*
            *       (ms*rs) + (mss*rss)
            * pos = ------------------
            *           (ms + mss)
            */
            double m_tmp = n->super_s->mass;
            n->super_s->mass += s->mass;        //increment mass super etoile
            vec *r_s = mul_vec( m_tmp, n->super_s->pos_t);
            vec *r_ss = mul_vec( s->mass, s->pos_t);
            vec *num_pos = add_vec(r_s, r_ss);
            double den_pos = 1 / n->super_s->mass;
            vec *vec_tmp = n->super_s->pos_t_dt;
            n->super_s->pos_t_dt = n->super_s->pos_t;
            vec *vec_mult_tmp = mul_vec( den_pos, num_pos);
            n->super_s->pos_t = vec_mult_tmp;
            for (int i = 0; i < NB_CHILDREN; ++i) {
                insert_star(n->children[i], s);
            }

            //liberation memoire
            free_vec(r_s);
            free_vec(r_ss);
            free_vec(vec_tmp);
            free_vec(num_pos);
        }
    }

}

quad_tree *create_quad_tree_from_galaxy(galaxy *g) {
    assert(g != NULL);
    quad_tree *new_quad_tree = calloc(1, sizeof(quad_tree));
    box *copy_box = new_box(g->b->x0, g->b->y0, g->b->w, g->b->h);
    new_quad_tree->root = node_create(NULL, copy_box);
    for (int i = 0; i < g->num_bodies; ++i) {
        insert_star(new_quad_tree->root, g->stars[i]);
    }
    return new_quad_tree;
}

void free_quad_tree(quad_tree *t) {     //liberation memoire
    assert(t != NULL);
    free_node(t->root);
    free(t);
}

void update_acceleration_from_node(const node *const n, star *s, double theta) {
    if (s == NULL || n == NULL) return;
    if (is_leaf(n) && n->s != s && n->is_empty == false) {
        update_acceleration(s, n->s);
    } else if (n->super_s != NULL && compute_length(n->b) / distance(n->super_s->pos_t, s->pos_t) < theta) {
        update_acceleration(s, n->super_s);
    } else {
        for (int i = 0; i < NB_CHILDREN; ++i) {
            update_acceleration_from_node(n->children[i], s, theta);
        }
    }
}

void update_accelerations_of_all_stars(quad_tree *t, galaxy *g, double theta) {
    assert(t != NULL);
    assert(g != NULL);
    for (int i = 0; i < g->num_bodies; ++i) {
        update_acceleration_from_node(t->root, g->stars[i], theta);     //si ni galaxy ni arbre vide alors update acc
    }
}

