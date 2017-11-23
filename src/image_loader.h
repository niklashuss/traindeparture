#pragma once

#include "image.h"

bool image_loader_load(const char* path, Image& image);
void image_loader_release(Image& image);
