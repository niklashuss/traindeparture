#pragma once

#include <SDL2/SDL.h>
#include "image.h"

class Texture {
public:
    Texture(SDL_Texture* pTexture);
    ~Texture();

    void lock(void** pData, int* pPitch);
    void unlock();

    void update(Image& image);
    SDL_Texture* getSdlTexture();

private:
    SDL_Texture* m_pTexture;
    int m_width;
    int m_height;
};
