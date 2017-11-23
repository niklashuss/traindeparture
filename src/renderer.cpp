#include "renderer.h"

void renderer_init()
{
}

void renderer_set_clear_color(float a, float r, float g, float b)
{
}

void renderer_clear()
{
}

void renderer_render(SDL_Renderer* pRenderer, Texture* pTexture, int x, int y) {
    SDL_Texture* pSdlTexture = pTexture->getSdlTexture();
    int width;
    int height;
    SDL_QueryTexture(pSdlTexture, nullptr, nullptr, &width, &height);
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = width;
    dest.h = height;
    SDL_RenderCopy(pRenderer, pSdlTexture, nullptr, &dest);
}
void renderer_execute()
{
}

void renderer_shutdown()
{
}
