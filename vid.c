#include <SDL.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define REDRAW_TIME 1000/30

struct vid_state {
    SDL_Surface     *screen;
    SDL_TimerID      timer;
    int              should_quit;
    uint8_t          frame;
};

void get_next_pixel(uint32_t x, uint32_t y, uint32_t frame,
                    uint8_t *r, uint8_t *g, uint8_t *b);

int redraw(struct vid_state *state) {
    SDL_Surface *screen = state->screen;
    uint32_t x, y;
    uint8_t r, g, b;
    int *pixels = (int *)screen->pixels;
    uint32_t px;

    uint32_t start = SDL_GetTicks();
    for(y=0; y<screen->h; y++) {
        for(x=0; x<screen->w; x++) {
            get_next_pixel(x, y, state->frame, &r, &g, &b);
            px = SDL_MapRGB(screen->format, r, g, b);
            pixels[y*screen->w+x] = px;
        }
    }
    SDL_Flip(screen);
    uint32_t end = SDL_GetTicks();
    if (!(state->frame % 60))
        fprintf(stderr, "Frame %d completed in %d msecs\n", state->frame,
                end-start);
    return 0;
}


static Uint32
signal_redraw(Uint32 interval, void *state)
{
    SDL_Event e;

    ((struct vid_state *)state)->frame++;

    /* Issue a redraw (if one isn't pending) */
    if (!SDL_PeepEvents(&e, 1, SDL_PEEKEVENT, SDL_ALLEVENTS)) {
        e.type = SDL_USEREVENT;
        e.user.code = 1;
        SDL_PushEvent(&e);
    }
    return interval;
}


int main(int argc, char **argv) {
    SDL_Event e;
    struct vid_state state;

    bzero(&state, sizeof(state));

    SDL_Init(SDL_INIT_EVERYTHING);
    state.screen = SDL_SetVideoMode(1280, 720, 32, 0);


    redraw(&state);
    state.timer = SDL_AddTimer(REDRAW_TIME, signal_redraw, &state);

    while (!state.should_quit) {
        SDL_WaitEvent(&e);
        switch(e.type) {

            case SDL_QUIT:
                state.should_quit = 1;
                break;

            case SDL_KEYDOWN: {
                SDL_KeyboardEvent *key = (SDL_KeyboardEvent *)&e;

                switch (key->keysym.sym) {
                    case SDLK_ESCAPE:
                        state.should_quit = 1;
                        SDL_Quit();
                        break;
                        /*
                    case SDLK_LEFT:
                        state.tx--;
                        redraw_noise(&state);
                        break;
                    case SDLK_RIGHT:
                        state.tx++;
                        redraw_noise(&state);
                        break;
                    case SDLK_UP:
                        state.ty--;
                        redraw_noise(&state);
                        break;
                    case SDLK_DOWN:
                        state.ty++;
                        redraw_noise(&state);
                        break;
                        */
                    default:
                        break;
                }
                break;
            }

            case SDL_USEREVENT:
                redraw(&state);
                break;

            default:
                break;
        }
    }

    return 0;
}
