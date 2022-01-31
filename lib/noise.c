#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"

#define MAXCOLOR 255

typedef struct __vec {
    double x, y;
} vec;

vec *new_vec(double x, double y) {
    vec *v = malloc(sizeof(vec));
    v->x = x;
    v->y = y;

    return v;
}

void show_pixels(struct gfx_context_t* context, int nx, int ny) {
    gfx_clear(context, COLOR_BLACK);

    for(int ix = 0; ix < nx; ++ix) {
		for(int iy = 0; iy < ny; ++iy){
            int px = rand() % MAXCOLOR;

            uint32_t color = MAKE_COLOR(px, px, px);

            vec *v = new_vec(ix, iy);

            gfx_putpixel(context, v->x, v->y, color);

            free(v);
        }
    }
    gfx_present(context);
}

int main()
{ 
    int nx = 600;
    int ny = 400;
    struct gfx_context_t* context = gfx_create("Random Image", nx, ny);
    if (!context) {
        fprintf(stderr, "Graphic mode initialization failed!\n");
        EXIT_FAILURE;
    }

    SDL_ShowCursor(SDL_ENABLE); // needed to se the cursor
    SDL_Keycode key_pressed = 0; // escape keyy needed

    int max_iter = 100000;

    int it = 0;
    while (it < max_iter) { // press escape to exit the loop
        SDL_PumpEvents();

        key_pressed = gfx_keypressed();
        if (key_pressed == SDLK_ESCAPE) {
            break;
        }

        show_pixels(context, nx, ny);
        
        it += 1;
    }

    gfx_destroy(context);

	return EXIT_SUCCESS;
}
