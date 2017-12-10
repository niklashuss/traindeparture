#pragma once

class Backlight {
public:
    static void on();
    static void off();
    static void brightness(int level);

private:
    Backlight() = delete;
    Backlight(const Backlight&) = delete;
    ~Backlight() = delete;
};
