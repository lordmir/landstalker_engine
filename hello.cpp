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
#include <memory>

#include "Graphics.h"
#include "VarWidthFont.h"
#include "TextLabel.h"

const std::string CHARSET = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*.,?!/<>:-'\"%#&()=[]{}@";
std::vector<int> char_widths;
const int char_height(15);
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
std::string input;

const std::filesystem::path assets_path(ASSETS_PATH);
std::shared_ptr<Font> main_font;
std::shared_ptr<Graphics> graphics;
std::vector<std::shared_ptr<IDrawable>> drawables;
std::shared_ptr<TextLabel> textedit;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    graphics = std::make_shared<Graphics>(800, 600, PROJECT_NAME);
    if (!graphics) {
        return SDL_APP_FAILURE;
    }
    main_font = std::make_shared<VarWidthFont>(
        std::make_shared<Texture>(graphics, std::filesystem::path(ASSETS_PATH "/graphics/fonts/mainfont.1bpp"), 16, 15, 1),
            CHARSET, std::unordered_map<char, Font::CONTROL_CHAR>{{'\n', Font::CONTROL_CHAR::NEWLINE}}, 8, 2);
    
        textedit = std::make_shared<TextLabel>(main_font, 1, 33, input, 2.0f);
        drawables.push_back(std::make_shared<TextLabel>(main_font, 1, 1, "Hello from Diamond-Shaped Dimension System 520!", 2.0f, 2.0f));
        drawables.push_back(textedit);
    
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    else if (event->type == SDL_EVENT_KEY_DOWN)
    {
        SDL_Keycode key = event->key.key;
        SDL_Keycode keycode = SDL_GetKeyFromScancode(event->key.scancode, event->key.mod, false);
        if (key == SDLK_ESCAPE)
        {
            return SDL_APP_SUCCESS;
        }
        else if (key == SDLK_DELETE || key == SDLK_BACKSPACE)
        {
            if(!input.empty())
            {
                input.pop_back();
            }
        }
        else if (key == SDLK_RETURN)
        {
            input += "\n";
        }
        else if (keycode <= 0xFF && main_font->IsValid(keycode))
        {
            input += (char)keycode;
        }
        textedit->SetLabelText(input);
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
    main_font.reset();
    graphics.reset();
}
