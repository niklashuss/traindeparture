#include "image.h"

Image image_create(int width, int height, int bytesPerPixel, int color)
{
    Image image;
    image.width = width;
    image.height = height;
    image.bpp = bytesPerPixel;
    int size = width * height * bytesPerPixel;
    image.data = new unsigned char[size];
    for (int i = 0; i < size; i++)
    {
        image.data[i] = color;
    }
    return image;
}

void image_clear(Image& image) {
    int size = image.width * image.height * image.bpp;
    image.data = new unsigned char[size];
    int color = 0x0;
    for (int i = 0; i < size; i++)
    {
        image.data[i] = color;
    }
}

void image_release(Image& image)
{
    delete[] image.data;
    image.data = nullptr;
    image.width = -1;
    image.height = -1;
    image.bpp = -1;
}


void image_copy(Image& source, Image& destination)
{
    int size = source.width * source.height * source.bpp;
    if (source.bpp == destination.bpp)
    {
        for (int i = 0; i < size; i++)
        {
            destination.data[i] = source.data[i];
        }
    }
    else if (source.bpp == 1 && destination.bpp == 2)
    {
        for (int i = 0; i < size; i++)
        {
            char color = source.data[i];
            destination.data[i * 2 + 0] = color;
            destination.data[i * 2 + 1] = color;
        }
    }
    else if (source.bpp == 1 && destination.bpp == 3)
    {
        for (int i = 0; i < size; i++)
        {
            char color = source.data[i];
            destination.data[i * 3 + 0] = color;
            destination.data[i * 3 + 1] = color;
            destination.data[i * 3 + 2] = color;
        }
    }
    else if (source.bpp == 1 && destination.bpp == 4)
    {
        for (int i = 0; i < size; i++)
        {
            char color = source.data[i];
            destination.data[i * 4 + 0] = color;
            destination.data[i * 4 + 1] = color;
            destination.data[i * 4 + 2] = color;
            destination.data[i * 4 + 3] = 1;
        }
    }
}

void image_make_transparent(Image& image, int color)
{
    if (image.bpp != 4) {
        return;
    }
    int size = image.width * image.height;
    int* ptr = reinterpret_cast<int*>(image.data);
    for (int i = 0; i < size; i++)
    {
        int color = ptr[i];
        if ((color & 0xffffff00) > 0 )
        {
            color = (color << 8) | 0xff;
        }
        else
        {
            color = 0xff;
        }
        ptr[i] = color;
    }
}
