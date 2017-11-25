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

void formatAsString(int format) {
    switch(format) {
        case SDL_PIXELFORMAT_UNKNOWN:
            printf("SDL_PIXELFORMAT_UNKNOWN\n");
            break;
        case SDL_PIXELFORMAT_RGB332:
            printf("SDL_PIXELFORMAT_RGB332\n");
            break;
        case SDL_PIXELFORMAT_RGB444:
            printf("SDL_PIXELFORMAT_RGB444\n");
            break;
        case SDL_PIXELFORMAT_RGB555:
            printf("SDL_PIXELFORMAT_RGB555\n");
            break;
        case SDL_PIXELFORMAT_BGR555:
            printf("SDL_PIXELFORMAT_BGR555\n");
            break;
        case SDL_PIXELFORMAT_ARGB4444:
            printf("SDL_PIXELFORMAT_ARGB4444\n");
            break;
        case SDL_PIXELFORMAT_RGBA4444:
            printf("SDL_PIXELFORMAT_RGBA4444\n");
            break;
        case SDL_PIXELFORMAT_ABGR4444:
            printf("SDL_PIXELFORMAT_ABGR4444\n");
            break;
        case SDL_PIXELFORMAT_BGRA4444:
            printf("SDL_PIXELFORMAT_BGRA4444\n");
            break;
        case SDL_PIXELFORMAT_ARGB1555:
            printf("SDL_PIXELFORMAT_ARGB1555\n");
            break;
        case SDL_PIXELFORMAT_RGBA5551:
            printf("SDL_PIXELFORMAT_RGBA5551\n");
            break;
        case SDL_PIXELFORMAT_ABGR1555:
            printf("SDL_PIXELFORMAT_ABGR1555\n");
            break;
        case SDL_PIXELFORMAT_BGRA5551:
            printf("SDL_PIXELFORMAT_BGRA5551\n");
            break;
        case SDL_PIXELFORMAT_RGB565:
            printf("SDL_PIXELFORMAT_RGB565\n");
            break;
        case SDL_PIXELFORMAT_BGR565:
            printf("SDL_PIXELFORMAT_BGR565\n");
            break;
        case SDL_PIXELFORMAT_RGB24:
            printf("SDL_PIXELFORMAT_RGB24\n");
            break;
        case SDL_PIXELFORMAT_BGR24:
            printf("SDL_PIXELFORMAT_BGR24\n");
            break;
        case SDL_PIXELFORMAT_RGB888:
            printf("SDL_PIXELFORMAT_RGB888\n");
            break;
        case SDL_PIXELFORMAT_RGBX8888:
            printf("SDL_PIXELFORMAT_RGBX8888\n");
            break;
        case SDL_PIXELFORMAT_BGR888:
            printf("SDL_PIXELFORMAT_BGR888\n");
            break;
        case SDL_PIXELFORMAT_BGRX8888:
            printf("SDL_PIXELFORMAT_BGRX8888\n");
            break;
        case SDL_PIXELFORMAT_ARGB8888:
            printf("SDL_PIXELFORMAT_ARGB8888\n");
            break;
        case SDL_PIXELFORMAT_RGBA8888:
            printf("SDL_PIXELFORMAT_RGBA8888\n");
            break;
        case SDL_PIXELFORMAT_ABGR8888:
            printf("SDL_PIXELFORMAT_ABGR8888\n");
            break;
        case SDL_PIXELFORMAT_BGRA8888:
            printf("SDL_PIXELFORMAT_BGRA8888\n");
            break;
        case SDL_PIXELFORMAT_ARGB2101010:
            printf("SDL_PIXELFORMAT_ARGB2101010\n");
            break;
        default:
            printf("Not covered: %d\n", format);
            break;
    }
}


Texture* Application::createStreamingTexture(int width, int height) {
    int requestFormat = SDL_PIXELFORMAT_ABGR8888;
    printf("Requested format: ");
    formatAsString(requestFormat);
    SDL_Texture* pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    unsigned int format;
    int access, w, h;
    SDL_QueryTexture(pTexture, &format, &access, &w, &h);
    printf("Create streaming picture: %x, format=%d, access=%d, w=%d, h=%d\n", pTexture, format, access, w, h);
    printf("Got format: ");
    formatAsString(format);
    return new Texture(pTexture);
}

