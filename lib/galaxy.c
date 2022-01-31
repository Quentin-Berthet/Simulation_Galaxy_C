//
// Created by quentin on 5/24/19.
//
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "galaxy.h"


double drand_f(double min, double max) {
    return ((double) rand() * (max - min)) / (double) RAND_MAX + min;
}

galaxy *create_and_init_galaxy(int num_bodies, box *b, double dt) {
    srand(time(NULL));
    //creer galaxy
    galaxy *new_galaxy = calloc(1, sizeof(galaxy));
    new_galaxy->b = b;
    new_galaxy->num_bodies = num_bodies;

    //first stars
    double mass_solar = 1.98892e30;
    vec *start_pos = new_vec(0.0, 0.0);
    vec *start_vel = new_vec(0.0, 0.0);
    double start_mass = 1e6 * mass_solar;
    new_galaxy->stars = calloc(num_bodies, sizeof(star *));
    new_galaxy->stars[0] = new_star_vel(start_pos, start_vel, new_vec(0.0, 0.0), start_mass, dt);  //1ere etoile

    free_vec(start_vel);
    //other stars


    double mass_min = 1e20;
    double R = 1e18;
    double G = 6.674e-11;
    for (int i = 1; i < num_bodies; ++i) {

        /*
         *            log( 1 - Rand(1) )     0.5 - Rand(1)
         * ri = R * (-------------------)  (              )
         *                 1.8               0.5 - Rand(1)
         */

        double random = drand_f(0.0, 10.0);        //chercher sur internet
        double const_pos = (log10(1 - drand_f(0.0, 1.0)) / 1.8);
        double pos_x = R * const_pos * (0.5 - drand_f(0.0, 1.0));  //position x d'une etoile
        double pos_y = R * const_pos * (0.5 - drand_f(0.0, 1.0));  //position y d'une etoile
        double angle = atan2(pos_y, pos_x);
        double mass = mass_min + random * mass_solar;   //masse de chaque etoile

        vec *pos = new_vec(pos_x, pos_y);    //pos de chaque etoile
        double norm_v = norm(pos);
        double const_vel = sqrt((G * (mass + start_mass)) / norm_v);
        vec *vel = new_vec(const_vel * (-sin(angle)), const_vel * cos(angle));  //vitesse de chaque etoile

        new_galaxy->stars[i] = new_star_vel(pos, vel, new_vec(0.0, 0.0), mass, dt);

        free_vec(vel);   //libere memoire

    }

    return new_galaxy;
}

star *new_rand_star(double dt) {
    double mass_solar = 1.98892e30;
    double start_mass = 1e6 * mass_solar;
    double mass_min = 1e20;
    double R = 1e18;
    double G = 6.674e-11;
    double random = drand_f(0.0, 10.0);        //chercher sur internet
    double mass = mass_min + random * mass_solar;   //masse de chaque etoile
    /*
     *        G * (mi + m0)        -sin(a)
     * vi = (--------------)^0.5  (       )
     *          ||ri||             cos(a)
     */
    double const_pos = (log10(1 - drand_f(0.0, 1.0)) / 1.8);
    double pos_x = R * const_pos * (0.5 - drand_f(0.0, 1.0));  //position x d'une etoile
    double pos_y = R * const_pos * (0.5 - drand_f(0.0, 1.0));  //position y d'une etoile
    double angle = atan2(pos_y, pos_x);     //angle  a = arctan(riy / rix)
    vec *pos = new_vec(pos_x, pos_y);    //pos de chaque etoile
    double norm_v = norm(pos);
    double const_vel = sqrt((G * (mass + start_mass)) / norm_v);
    vec *vel = new_vec(const_vel * (-sin(angle)), const_vel * cos(angle));  //vitesse de chaque etoile
    star *new_star = new_star_vel(pos, vel, new_vec(0.0, 0.0), mass, dt);
    free_vec(vel);      //libere memoire
    return new_star;
}


void reset_accelerations(galaxy *g) {
    for (int i = 0; i < g->num_bodies; ++i) {
        reset_acceleration(g->stars[i]);     //appels la fonction rest_acc_(star) pour toute les etoile
    }

}

void update_positions(galaxy *g, double dt) {
    for (int i = 1; i < g->num_bodies; ++i) {
        update_position(g->stars[i], dt);      //appels la fonction update_pos_(star) pour toute les etoile
    }
}

void free_galaxy(galaxy *g) {
    for (int i = 0; i < g->num_bodies; ++i) {
        if (g->stars[i] != NULL) {
            free_star(g->stars[i]); //libere le contenu de  etoile
            g->stars[i] = NULL;
        }
    }
    free(g->stars); //libere etoile
    free_box(g->b); //libere box
    free(g);    //libere galaxy
}

void resize_galaxy(galaxy *g) {
    for (int i = 0; i < g->num_bodies; ++i) {
        if (g->stars[i] == NULL || is_inside(g->b, g->stars[i]->pos_t)) {      //si dans la boite ou NULL rien
            continue;
        } else {
            free_star(g->stars[i]); //libere etoile
            g->stars[i] = new_rand_star(1e10);  //genere etoile aleatoir
            resize_galaxy(g);   //redimentionne galaxy
        }
    }
}
