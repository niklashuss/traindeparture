#include "backlight.h"
#include <cstdlib>
#include <cstdio>

void Backlight::on() {
    system("sudo bash -c \"echo 0 > /sys/class/backlight/rpi_backlight/bl_power\"");
}

void Backlight::off() {
    system("sudo bash -c \"echo 1 > /sys/class/backlight/rpi_backlight/bl_power\"");
}

void Backlight::brightness(int level) {
    level = level < 0 ? 0 : level;
    level = level > 100 ? 100 : level;

    char buffer[256];
    sprintf(buffer, "sudo bash -c \"echo %d > /sys/class/backlight/rpi_backlight/brightness\"", level);
    system(buffer);
}
