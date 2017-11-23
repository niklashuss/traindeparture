#pragma once

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <stb_truetype.h>
#include "image.h"

struct Font
{
    stbtt_fontinfo font;
    int baseline;
    float scale;
    unsigned char* buffer;
};

void font_load(Font& font, const char *path, int height);
void font_render(Font& font, Image& image, const char* text);
void font_release(Font& font);

