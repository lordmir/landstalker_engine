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
#include "Timer.h"
#include "VarWidthFont.h"
#include "TextLabel.h"
#include "TextInput.h"
#include "Keyboard.h"
#include "Palette.h"
#include "TextBox.h"
#include "InputBox.h"
#include "FpsBox.h"
#include "HudDemo.h"

const std::string CHARSET = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*.,?!/<>:-'\"%#&()=_+[]@";
std::shared_ptr<Graphics> graphics;
std::vector<std::shared_ptr<IDrawable>> drawables;
std::shared_ptr<InputBox> inputbox;
std::shared_ptr<TextBox> fps;
std::shared_ptr<Hud> hud;

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
        CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}}, 8, 2, 1);
    auto menu_font = std::make_shared<Font>(
        std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/fonts/menufont.1bpp"), 8, 8, 1),
        CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}});
    auto ui_tiles = std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/tilesets/ui.bin"), 8, 8, 4);
    auto ui_pal = std::make_shared<Palette>(std::filesystem::path(ASSETS_PATH "/graphics/palettes/ui.pal"), 10, true);
    ui_pal->SetSdlColour(15, {0, 0, 0, 0}); // Make index 15 fully transparent
    ui_tiles->SetColours(ui_pal->GetSdlColours());
    menu_font->SetColours(ui_pal->GetSdlColours());
    main_font->SetColours(ui_pal->GetSdlColours());

    inputbox = std::make_shared<InputBox>(ui_tiles, ui_pal, main_font, "Type something: ");
    inputbox->SetExternalWidthPixels(graphics->GetWidth());
    inputbox->SetInternalHeightPixels(inputbox->GetCharHeight() * 5);
    inputbox->SetY(graphics->GetHeight() - inputbox->GetExternalHeightPixels() - inputbox->GetCharHeight());
    inputbox->SetMaxSize(inputbox->GetInternalWidthPixels(), inputbox->GetInternalHeightPixels());
    inputbox->SetWrapEnabled(true);
    
    fps = std::make_shared<FpsBox>(ui_tiles, ui_pal, menu_font, 0, 0);
    hud = std::make_shared<HudDemo>(ui_tiles, ui_pal, 0, 0, graphics->GetWidth() - fps->GetExternalWidthPixels() - 32, 8);
    fps->SetX(hud->GetExternalWidthPixels());
    SDL_Rect main_viewport = {
        0,
        static_cast<int>(hud->GetExternalHeightPixels()),
        static_cast<int>(graphics->GetWidth()),
        static_cast<int>(graphics->GetHeight() - hud->GetExternalHeightPixels() - inputbox->GetExternalHeightPixels())
    };
    auto hello = std::make_shared<TextBox>(ui_tiles, ui_pal, main_font, "Hello from Diamond-Shaped Dimension System 520!", main_viewport.x, main_viewport.y, 0, 0, true);
    hello->SetX((main_viewport.w - hello->GetExternalWidthPixels()) / 2);
    hello->SetY((main_viewport.h - hello->GetExternalHeightPixels()) / 2);
    drawables.push_back(hello);
    drawables.push_back(inputbox);
    drawables.push_back(hud);
    drawables.push_back(fps);
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
    const double delta = Timer::GetDelta();
    std::for_each(drawables.begin(), drawables.end(), [delta](auto& d){ d->OnUpdate(delta); });
    Timer::ProcessTicks([](unsigned int tick_count)
    {
        std::for_each(drawables.begin(), drawables.end(), [tick_count](auto& d){ d->OnTick(tick_count); });
    });
    graphics->Render(drawables);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
