#include "backlight.h"
#include <cstdlib>
#include <cstdio>

void Backlight::on() {
#ifdef __arm__
    system("sudo bash -c \"echo 0 > /sys/class/backlight/rpi_backlight/bl_power\"");
#endif
}

void Backlight::off() {
#ifdef __arm__
    system("sudo bash -c \"echo 1 > /sys/class/backlight/rpi_backlight/bl_power\"");
#endif
}

void Backlight::brightness(int level) {
#ifdef __arm__
    level = level < 0 ? 0 : level;
    level = level > 100 ? 100 : level;

    char buffer[256];
    sprintf(buffer, "sudo bash -c \"echo %d > /sys/class/backlight/rpi_backlight/brightness\"", level);
    system(buffer);
#endif
}
