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

#include "Graphics.h"
#include "VarWidthFont.h"
#include "TextLabel.h"
#include "TextInput.h"
#include "Keyboard.h"

const std::string CHARSET = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*.,?!/<>:-'\"%#&()=[]{}@";
const std::filesystem::path assets_path(ASSETS_PATH);
std::shared_ptr<Graphics> graphics;
std::vector<std::shared_ptr<IDrawable>> drawables;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    graphics = std::make_shared<Graphics>(800, 600, PROJECT_NAME);
    if (!graphics) {
        return SDL_APP_FAILURE;
    }
    auto main_font = std::make_shared<VarWidthFont>(
        std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/fonts/mainfont.1bpp"), 16, 15, 1),
            CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}}, 8, 2);
    auto menu_font = std::make_shared<Font>(
        std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/fonts/menufont.1bpp"), 8, 8, 1),
            CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}});
            
    auto textinput = std::make_shared<TextInput>(menu_font, 1, 33, "Type something: ", 2.0f);
    drawables.push_back(std::make_shared<TextLabel>(main_font, 1, 1, "Hello from Diamond-Shaped Dimension System 520!", 2.0f, 2.0f));
    drawables.push_back(textinput);
    Keyboard::GetInstance().RegisterKeyInputHandler(textinput);
    
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
    graphics->Render(drawables);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
