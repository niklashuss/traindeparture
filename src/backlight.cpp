#include "backlight.h"
#include <cstdlib>
#include <cstdio>

#define USE_BACKLIGHT

void Backlight::on() {
    #ifdef USE_BACKLIGHT
    system("sudo bash -c \"echo 0 > /sys/class/backlight/rpi_backlight/bl_power\"");
#else
    printf("Backlight: on()\n");
#endif
}

void Backlight::off() {
#ifdef USE_BACKLIGHT
    system("sudo bash -c \"echo 1 > /sys/class/backlight/rpi_backlight/bl_power\"");
#else
    printf("Backlight: off()\n");
#endif
}

void Backlight::brightness(int level) {
#ifdef USE_BACKLIGHT
    level = level < 0 ? 0 : level;
    level = level > 100 ? 100 : level;

    char buffer[256];
    sprintf(buffer, "sudo bash -c \"echo %d > /sys/class/backlight/rpi_backlight/brightness\"", level);
    system(buffer);
#else
    printf("Backlight: brightness(%d)\n", level);
#endif
}
