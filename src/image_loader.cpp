#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool image_loader_load(const char* path, Image& image)
{
    image.data = stbi_load(path, &image.width, &image.height, &image.bpp, 0);
    if (image.data == nullptr)
    {
        return false;
    }
    return true;
}

void image_loader_release(Image& image)
{
    stbi_image_free(image.data);
}
