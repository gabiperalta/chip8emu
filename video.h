#ifndef VIDEO_H
#define VIDEO_H
#include <stdbool.h>
#include <SDL2/SDL_keycode.h>

#define SCREEN_SCALE 10

void init_video();
int key_event();
void show_video();
void draw(bool *pixels);
void close_video();

void set_cycle_test(const bool value);
bool get_cycle_test();
void set_key_pressed(SDL_Keycode value);
SDL_Keycode get_key_pressed();

#endif
