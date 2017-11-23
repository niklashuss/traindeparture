#include "font.h"
#include "file.h"

void font_load(Font& font, const char *path, int height)
{
    int size = File::size(path);
    font.buffer = new unsigned char[size];
    {
        File file;
        file.open(path, FileMode::Read);
        file.read(reinterpret_cast<char*>(font.buffer), size);
    }
    stbtt_InitFont(&font.font, const_cast<const unsigned char*>(font.buffer), 0);

    font.scale = stbtt_ScaleForPixelHeight(&font.font, height);

    int ascent;
    stbtt_GetFontVMetrics(&font.font, &ascent, 0, 0);
    font.baseline = (int)(ascent * font.scale);
}

void font_render(Font& font, Image& image, const char* text)
{
    int ch = 0;
    float xpos = 2;
    while (text[ch])
    {
        int advance, lsb, x0, y0, x1, y1;
        float x_shift = xpos - (float) floor(xpos);
        stbtt_GetCodepointHMetrics(&font.font, text[ch], &advance, &lsb);
        stbtt_GetCodepointBitmapBoxSubpixel(&font.font, text[ch], font.scale, font.scale, x_shift, 0, &x0, &y0, &x1, &y1);
        int x = (int)xpos + x0;
        int y = (font.baseline + y0);
        stbtt_MakeCodepointBitmapSubpixel(&font.font, &image.data[x + y * image.width], x1-x0, y1-y0, image.width, font.scale, font.scale, x_shift, 0, text[ch]);
        xpos += (advance * font.scale);
        if (text[ch+1])
        {
            xpos += font.scale*stbtt_GetCodepointKernAdvance(&font.font, text[ch],text[ch+1]);
        }
        ++ch;
    }
}

void font_release(Font& font) {
    delete[] font.buffer;
}