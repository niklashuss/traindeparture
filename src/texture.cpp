#include "texture.h"

Texture::Texture(SDL_Texture* pTexture)
        : m_pTexture(pTexture) {
    SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_ADD);
    SDL_QueryTexture(m_pTexture, nullptr, nullptr, &m_width, &m_height);
}

Texture::~Texture() {
    SDL_DestroyTexture(m_pTexture);
    m_pTexture = nullptr;
}

void Texture::lock(void** pData, int* pPitch) {
    SDL_LockTexture(m_pTexture, nullptr, pData, pPitch);
}

void Texture::unlock() {
    SDL_UnlockTexture(m_pTexture);
}

void Texture::update(Image &image) {
    SDL_UpdateTexture(m_pTexture, nullptr, image.data, image.bpp * image.width);
}

SDL_Texture* Texture::getSdlTexture() {
    return m_pTexture;
}
