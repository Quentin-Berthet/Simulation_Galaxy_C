#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "quad_tree.h"
#include "galaxy.h"
#include "star.h"
#include "vector.h"
#include "box.h"
#include "gfx.h"

const int SIZE_W = 1000; //800;
const int SIZE_H = 800; //600;



int main(int argc, char *argv[]) {
    assert(argc == 3);
    int nb_stars = atoi(argv[1]);
    double theta = atof(argv[2]);


    struct gfx_context_t *context = gfx_create("Random Galaxy", SIZE_W, SIZE_H);
    if (!context) {
        fprintf(stderr, "Graphic mode initialization failed!\n");
        EXIT_FAILURE;
    }

    SDL_ShowCursor(SDL_ENABLE); // needed to se the cursor

    galaxy *galaxy = create_and_init_galaxy(nb_stars, new_box(-5e17, -5e17, 10e17, 10e17), 1e10);


    SDL_Keycode key_pressed = 0; // escape keyy needed
    double scale_x = galaxy->b->w / SIZE_W;
    double scale_y = galaxy->b->h / SIZE_H;
    while (true) { // press escape to exit the loop
        SDL_PumpEvents();
        resize_galaxy(galaxy);
        key_pressed = gfx_keypressed();
        if (key_pressed == SDLK_ESCAPE) {
            break;
        }
        gfx_clear(context, COLOR_BLACK);
        for (int i = 0; i < galaxy->num_bodies; ++i) {
            if (galaxy->stars[i] != NULL) {
                double x_tmp = galaxy->stars[i]->pos_t->x - (-5e17);
                double y_tmp = galaxy->stars[i]->pos_t->y - (-5e17);
                double x_scale = x_tmp / scale_x;
                double y_scale = y_tmp / scale_y;
                gfx_putpixel(context, (int) x_scale, (int) y_scale, COLOR_GREEN);
            }
        }
        gfx_present(context);
        quad_tree *q_t = create_quad_tree_from_galaxy(galaxy);
        reset_accelerations(galaxy);
        update_accelerations_of_all_stars(q_t, galaxy, theta);
        update_positions(galaxy, 1e10);
        free_quad_tree(q_t);
    }
    gfx_destroy(context);
    free_galaxy(galaxy);
    return EXIT_SUCCESS;
}


