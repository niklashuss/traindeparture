#pragma once

#include <SDL2/SDL.h>
#include "texture.h"

class Application {
public:
    enum class Status {
        Success,
        InitFailed,
        CreateWindowFailed,
        CreateRendererFailed
    };

    Application();

    ~Application();

    Status create(int width, int height);

    void execute();

    virtual void onInit() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    virtual void onShutdown() = 0;
    virtual void onKeyEvent(int key, bool pressed) = 0;

    Texture* createStreamingTexture(int width, int height);
    void render(Texture* pTexture, int x, int y);
    void drawLine(Texture* pTexture, int x, int y, int width, int height);

private:
    SDL_Window* m_pWindow = nullptr;
    SDL_Renderer* m_pRenderer = nullptr;
    Status m_status = Status::Success;
    bool m_shouldLoop = true;

    void handleKeyEvent(SDL_KeyboardEvent& keyEvent);
};
