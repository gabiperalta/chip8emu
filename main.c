#include "cpu.h"
#include "video.h"

int main(int argc, char *argv[]) {

    init_cpu();
    init_video();

    if (argc == 2) {
        load_program(argv[1]);
        //printf("argc %d\n", argc);
        //printf("argv %s\n", argv[1]);
    } else {
        printf("No filename has specified\n");
        return -1;
    }

    //bool *pixels_test = malloc(sizeof(bool) * 64 * 32);
    //memset(pixels_test, 0, sizeof(bool) * 64 * 32);
    //pixels_test[0 + 0 * 64] = true;
    //pixels_test[0 + 1 * 64] = true;

    //draw(pixels_test);

    ///*
    SDL_Keycode key_pressed;
    while ((key_pressed = key_event()) != SDLK_ESCAPE) {

        set_key_pressed(key_pressed);

        //if (SDLK_UP == key_pressed) { // key arrow UP
            cpu_fetch();
            cpu_decode();
            cpu_execute();
        //}

        draw(get_pixels());
        show_video();
    }

    close_video();
    close_cpu();
    //*/
    return 0;
}
