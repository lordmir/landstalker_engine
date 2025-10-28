#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <format>

#include "Graphics.h"
#include "VarWidthFont.h"
#include "TextLabel.h"
#include "TextInput.h"
#include "Keyboard.h"
#include "Palette.h"
#include "TextBox.h"
#include "InputBox.h"

const std::string CHARSET = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*.,?!/<>:-'\"%#&()=[]{}@";
std::shared_ptr<Graphics> graphics;
std::vector<std::shared_ptr<IDrawable>> drawables;
std::shared_ptr<InputBox> inputbox;
std::shared_ptr<TextBox> fps;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    graphics = std::make_shared<Graphics>(1280, 1024, PROJECT_NAME);
    if (!graphics) {
        return SDL_APP_FAILURE;
    }
    auto main_font = std::make_shared<VarWidthFont>(
        std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/fonts/mainfont.1bpp"), 16, 15, 1),
        CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}}, 8, 2);
    auto menu_font = std::make_shared<Font>(
        std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/fonts/menufont.1bpp"), 8, 8, 1),
        CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}});
    auto ui_tiles = std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/tilesets/ui.bin"), 8, 8, 4);
    auto ui_pal = std::make_shared<Palette>(std::filesystem::path(ASSETS_PATH "/graphics/palettes/ui.pal"), 10, true);
    ui_pal->SetSdlColour(15, {0, 0, 0, 0}); // Make index 15 fully transparent
    ui_tiles->SetColours(ui_pal->GetSdlColours());
    menu_font->SetColours(ui_pal->GetSdlColours());
    main_font->SetColours(ui_pal->GetSdlColours());

    inputbox = std::make_shared<InputBox>(ui_tiles, ui_pal, main_font, "Type something: ", 16, 40, 1, 1, true);
    fps = std::make_shared<TextBox>(ui_tiles, ui_pal, menu_font, "  0 FPS", 1100, 0, 1, 1, true);
    drawables.push_back(std::make_shared<TextBox>(ui_tiles, ui_pal, main_font, "Hello from Diamond-Shaped Dimension System 520!", 0, 0, 50, 1, true));
    drawables.push_back(fps);
    drawables.push_back(inputbox);
    Keyboard::GetInstance().RegisterKeyInputHandler(inputbox);
    
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    else if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
    {
        auto keycode = SDL_GetKeyFromScancode(event->key.scancode, event->key.mod, false);
        if(event->type == SDL_EVENT_KEY_DOWN && keycode == SDLK_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
        Keyboard::GetInstance().OnKeyEvent(event->key);
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    static SDL_Color original = inputbox->GetBackgroundColour();
    static SDL_Color target = {0xFF, 0x00, 0x00, 0xFF};
    static float factor = 0.0f;
    static bool direction = false;
    static uint64_t last_time = 0ULL;
    uint64_t current_time = SDL_GetPerformanceCounter();
    double delta = (current_time - last_time) / static_cast<double>(SDL_GetPerformanceFrequency());
    last_time = current_time;
    const float step = delta == 0ULL ? 0.0f : delta / 5.0;
    
    factor += step * (direction ? 1.0f : -1.0f);
    if(factor >= 1.0f)
    {
        factor = 1.0f;
        direction = false;
    }
    else if(factor <= 0.0f)
    {
        factor = 0.0f;
        direction = true;
    }

    fps->SetLabelText(std::format("{0:4d} FPS", static_cast<unsigned int>(1.0 / delta)));

    SDL_Color blended = {
        static_cast<Uint8>(std::clamp<int>(original.r + factor * (target.r - original.r), 0, 255)),
        static_cast<Uint8>(std::clamp<int>(original.g + factor * (target.g - original.g), 0, 255)),
        static_cast<Uint8>(std::clamp<int>(original.b + factor * (target.b - original.b), 0, 255)),
        0xFF
    };

    inputbox->SetBackgroundColour(blended);
    graphics->Render(drawables);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
