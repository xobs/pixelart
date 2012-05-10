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

int redraw(struct vid_state *state) {
    SDL_Surface *screen = state->screen;
    int x, y;
    int r, g, b;
    int *pixels = (int *)screen->pixels;
    uint32_t px;

    r = 0;
    g = 255;
    b = 0;
    for(y=0; y<screen->h; y++) {
        for(x=0; x<screen->w; x++) {
            px = SDL_MapRGB(screen->format, r, g, b);
            pixels[y*screen->w+x] = px;
        }
    }
    SDL_Flip(screen);
    return 0;
}


static Uint32
signal_redraw(Uint32 interval, void *state)
{
    SDL_Event e;

    ((struct vid_state *)state)->frame++;

    /* Issue a redraw */
    e.type = SDL_USEREVENT;
    e.user.code = 1;
    SDL_PushEvent(&e);
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
