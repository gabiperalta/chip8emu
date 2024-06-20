#ifndef VIDEO_H
#define VIDEO_H
#include <stdbool.h>

#define SCREEN_SCALE 10

void init_video();
bool key_event();
void show_video();
void draw(bool *pixels);
void close_video();

void set_cycle_test(const bool value);
bool get_cycle_test();

#endif
