// filepath: c:\PROJECTS\sdl3_ls_engine\Graphics.cpp
#include "Graphics.h"
#include <SDL3/SDL.h>
#include <stdexcept>

Graphics::Graphics(int width, int height, const std::string& title)
    : width(width), height(height), window(nullptr), renderer(nullptr)
{
    // Create the window
    if (!SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        throw std::runtime_error(SDL_GetError());
    }

    const SDL_Rect textarea = {0, 0, 800, 600};
    SDL_SetTextInputArea(window, &textarea, 0);
    SDL_StartTextInput(window);
}

Graphics::~Graphics()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    // Quit SDL subsystems initialized by this object
    SDL_Quit();
}

void Graphics::OnRender()
{
    if (!renderer) return;

    // Clear to black and present (simple stub)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // TODO: add drawing code here

    SDL_RenderPresent(renderer);
}
