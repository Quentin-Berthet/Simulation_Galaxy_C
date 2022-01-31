//
// Created by quentin on 5/23/19.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "star.h"

star *new_star_vel(vec *pos, vec *vel, vec *acc, double mass, double dt) {
    star *new_star = calloc(1, sizeof(star));
    new_star->acc = acc;
    new_star->mass = mass;
    new_star->pos_t = pos;
    vec *tmp_vec = mul_vec(dt, vel);
    new_star->pos_t_dt = sub_vec(new_star->pos_t, tmp_vec);
    free_vec(tmp_vec);
    return new_star;
}

void reset_acceleration(star *s) {
    if (s == NULL) {
        return;
    }
    s->acc->x = 0.0;
    s->acc->y = 0.0;
}

void update_acceleration(star *s, star *s2) {
    if (s == NULL || s2 == NULL) return;
    double m1 = s->mass;
    double m2 = s2->mass;
    double G = 6.67e-11;

    /*
     *        G * mi * mj * (rj - ri)
     * Fij = ------------------------
     *           (rij +  3e4)³
     */

    vec *r_ji = sub_vec(s2->pos_t, s->pos_t);
    vec *dist_vec = sub_vec(s2->pos_t, s->pos_t);
    vec *num_force_1_2 = mul_vec(m1 * m2 * G, r_ji);
    double denum_force_1_2 = (1 / pow(norm(dist_vec) + 3e4, 3));
    vec *force_1_2 = mul_vec(denum_force_1_2, num_force_1_2);
    s->acc->x += force_1_2->x / m1;
    s->acc->y += force_1_2->y / m1;

    //liberation memoire
    free_vec(r_ji);
    free_vec(dist_vec);
    free_vec(num_force_1_2);
    free_vec(force_1_2);
}

void update_position(star *s, double dt) {
    /*
     * r(t + t') = 2 * r(t) - r(t - t') + t'² * a(t)
     */

    if (s == NULL) return;
    double pos_x_tmp = s->pos_t_dt->x;
    double pos_y_tmp = s->pos_t_dt->y;
    s->pos_t_dt->x = s->pos_t->x;
    s->pos_t_dt->y = s->pos_t->y;
    s->pos_t->x = 2 * s->pos_t->x - pos_x_tmp + s->acc->x * pow(dt, 2);
    s->pos_t->y = 2 * s->pos_t->y - pos_y_tmp + s->acc->y * pow(dt, 2);
}

void print_star(star *s) {
    print_vec(s->acc);
    print_vec(s->pos_t);
    print_vec(s->pos_t_dt);
    printf("masse %g", s->mass);
}

void free_star(star *s) {
    if (s == NULL) return;
    free_vec(s->pos_t);
    free_vec(s->pos_t_dt);
    free_vec(s->acc);
    free(s);
}
