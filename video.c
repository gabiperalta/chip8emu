#include "video.h"

#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool cycle_test;
SDL_Keycode key_pressed;

void init_video() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // or INIT EVERYTHING
        printf("SDL_Init Error: %s\n", SDL_GetError());
        //return 1;
    }

    window = SDL_CreateWindow("CHIP8 Emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64 * SCREEN_SCALE,
        32 * SCREEN_SCALE, 0);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        //return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        //return 1;
    }

    SDL_RenderSetScale(renderer, SCREEN_SCALE, SCREEN_SCALE);
}

int key_event() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            return event.key.keysym.sym;
        }
    }
    return SDLK_UNKNOWN;
}

void show_video() {
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

void draw(bool *pixels) {
    /*
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 0, 32/2);

    SDL_RenderPresent(renderer);

    SDL_Delay(10); // Wait for 2 seconds
    */

    for (int x = 0; x < 64; x++) {
        for(int y = 0; y < 32; y++) {
            if (pixels[x + y * 64]) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void close_video() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void set_cycle_test(bool value) {
    cycle_test = value;
}

bool get_cycle_test() {
    return cycle_test;
}

void set_key_pressed(SDL_Keycode value) {
    key_pressed = value;
}

SDL_Keycode get_key_pressed() {
    return key_pressed;
}
