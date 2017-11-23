#pragma once

#include <SDL2/SDL.h>
#include "texture.h"

void renderer_init();

void renderer_set_clear_color(float a, float r, float g, float b);
void renderer_clear();
void renderer_execute();
void renderer_render(SDL_Renderer* pRenderer, Texture* pTexture, int x, int y);
void renderer_shutdown();
