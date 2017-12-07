#pragma once

struct Image
{
  int width;
  int height;
  int bytesPerPixel;
  unsigned char* data;
};

Image image_create(int width, int height, int bytesPerPixel, int color = 0);

void image_clear(Image& image);

void image_release(Image& image);

void image_copy(Image& source, Image& destination);

void image_make_transparent(Image& image, int color);

