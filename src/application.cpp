#include "application.h"
#include <stdio.h>
#include "renderer.h"

Application::Application() : m_status(Application::Status::Success) {
    printf("Application::settings up\n");
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Failed to initialized SDL: %s\n", SDL_GetError());
        m_status = Status::InitFailed;
    }
}

Application::~Application() {
    if (m_pRenderer != nullptr) {
        SDL_DestroyRenderer(m_pRenderer);
    }
    if (m_pWindow != nullptr) {
        SDL_DestroyWindow(m_pWindow);
    }
    m_pRenderer = nullptr;
    m_pWindow = nullptr;
    SDL_Quit();
}

Application::Status Application::create(int width, int height) {
    if (m_status != Status::Success) {
        return m_status;
    }

    int flags = SDL_WINDOW_OPENGL;
    m_pWindow = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (m_pWindow == nullptr) {
        return Status::CreateWindowFailed;
    }

    int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, renderFlags);
    if (m_pRenderer == nullptr) {
        return Status::CreateRendererFailed;
    }

    SDL_DisableScreenSaver();
    SDL_ShowCursor(SDL_DISABLE);
    return m_status;
}
void Application::handleKeyEvent(SDL_KeyboardEvent& keyEvent)
{
    if (keyEvent.keysym.sym == SDLK_ESCAPE) {
        m_shouldLoop = false;
    } else {
        if (keyEvent.state == SDL_KEYDOWN) {
            onKeyEvent(keyEvent.keysym.sym, true);
        } else {
            onKeyEvent(keyEvent.keysym.sym, false);
        }
    }
}

void Application::execute() {
    SDL_Event event;
    onInit();
    while (m_shouldLoop) {
        SDL_PollEvent(&event);
        handleKeyEvent(event.key);
        if (event.type == SDL_QUIT) {
            m_shouldLoop = false;
            break;
        }

        onUpdate();
        SDL_SetRenderDrawColor(m_pRenderer, 0x88, 0x88, 0xff, 0x00);
        SDL_RenderClear(m_pRenderer);
        onRender();

        SDL_RenderPresent(m_pRenderer);
    }
    onShutdown();
}

void Application::render(Texture* pTexture, int x, int y) {
    renderer_render(m_pRenderer, pTexture, x, y);
}

Texture* Application::createStreamingTexture(int width, int height) {
    int requestFormat = SDL_PIXELFORMAT_ABGR8888;
    SDL_Texture* pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    unsigned int format;
    int access, w, h;
    SDL_QueryTexture(pTexture, &format, &access, &w, &h);
    return new Texture(pTexture);
}

