#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cpu.h"
#include "video.h"

int main(void) {

    bool run = true;
    //bool cycle_test = false;

    init_cpu();
    init_video();

    load_program();

    //bool *pixels_test = malloc(sizeof(bool) * 64 * 32);
    //memset(pixels_test, 0, sizeof(bool) * 64 * 32);
    //pixels_test[0 + 0 * 64] = true;
    //pixels_test[0 + 1 * 64] = true;

    //draw(pixels_test);

    while (run) {

        if (get_cycle_test()) {
            cpu_fetch();
            cpu_decode();
            cpu_execute();
            set_cycle_test(true);
        }

        run = key_event();

        draw(get_pixels());
        show_video();
    }

    close_video();
    close_cpu();
    return 0;
}
