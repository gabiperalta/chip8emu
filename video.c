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

uint8_t get_hex_from_ascii(SDL_Keycode value) {
    switch (value) {
        case SDLK_0:
            return 0x0;
        case SDLK_1:
            return 0x1;
        case SDLK_2:
            return 0x2;
        case SDLK_3:
            return 0x3;
        case SDLK_4:
            return 0x4;
        case SDLK_5:
            return 0x5;
        case SDLK_6:
            return 0x6;
        case SDLK_7:
            return 0x7;
        case SDLK_8:
            return 0x8;
        case SDLK_9:
            return 0x9;
        case SDLK_a:
            return 0xA;
        case SDLK_b:
            return 0xB;
        case SDLK_c:
            return 0xC;
        case SDLK_d:
            return 0xD;
        case SDLK_e:
            return 0xE;
        case SDLK_f:
            return 0xF;
        default:
            return 0x0;
    }
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
