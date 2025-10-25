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

const std::string CHARMAP = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*.,?!/<>:-'\"%#&()=[]{}@";
std::vector<int> char_widths;
const int char_height(15);
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
SDL_Texture* main_font;
std::string input;

const std::filesystem::path assets_path(ASSETS_PATH);

std::vector<uint8_t> read_bytes(const std::filesystem::path& path)
{
    std::vector<uint8_t> bytes = {};
    std::ifstream file(path, std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    bytes.reserve(file_size);
    
    bytes.insert(bytes.begin(),
                 std::istream_iterator<uint8_t>(file),
                 std::istream_iterator<uint8_t>());

    return bytes;
}

static inline unsigned int trailing_zeroes(int n) {
    unsigned bits = 0, x = n;

    if (x) {
        /* assuming `x` has 32 bits: lets count the low order 0 bits in batches */
        /* mask the 16 low order bits, add 16 and shift them out if they are all 0 */
        if (!(x & 0x0000FFFF)) { bits += 16; x >>= 16; }
        /* mask the 8 low order bits, add 8 and shift them out if they are all 0 */
        if (!(x & 0x000000FF)) { bits += 8; x >>= 8; }
        /* mask the 4 low order bits, add 4 and shift them out if they are all 0 */
        if (!(x & 0x0000000F)) { bits += 4; x >>= 4; }
        /* mask the 2 low order bits, add 2 and shift them out if they are all 0 */
        if (!(x & 0x00000003)) { bits += 2; x >>= 2; }
        /* mask the low order bit and add 1 if it is 0 */
        bits += (x & 1) ^ 1;
    }
    return bits;
}

SDL_Texture* load_tex_from_bytes(const std::vector<uint8_t>& bytes, unsigned int tile_w, unsigned int tile_h, unsigned int bitdepth)
{
    unsigned int pixels_per_byte = 8 / bitdepth;
    unsigned int total_h = bytes.size() * pixels_per_byte / tile_w;
    unsigned int total_c = total_h / tile_h;
    unsigned int bytes_per_c = tile_w * tile_h / pixels_per_byte;

    char_widths.reserve(total_c);
    char_widths.push_back(8);
    auto l = bytes.data() + bytes_per_c;
    for (unsigned int i = 1; i < total_c; ++i)
    {
        int width = 1;
        for (unsigned int j = 0; j < tile_h; ++j)
        {
            uint16_t line = (l[0] << 8) | l[1];
            if (line == 0)
            {
                l += 2;
                continue;
            }
            int l_width = 18 - trailing_zeroes(line);
            width = std::max(width, l_width);
            l += 2;
        }
        char_widths.push_back(width);
    }
    auto surface = SDL_CreateSurfaceFrom(tile_w, total_h, SDL_PIXELFORMAT_INDEX1MSB, const_cast<uint8_t*>(bytes.data()), tile_w / pixels_per_byte);
    SDL_Color c[] = {{0,0,0,0}, {0xFF, 0xFF, 0xFF, 0xFF}};
    auto pal = SDL_CreatePalette(2);
    pal->colors[0] = { 0, 0, 0, 0xFF };
    pal->colors[1] = { 0xFF, 0xFF, 0xFF, 0 };
    SDL_SetSurfacePalette(surface, pal);
    auto tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_PIXELART);
    return tex;
}

int write_char(char c, int x, int y, int scale=1)
{
    auto res = CHARMAP.find(c);
    if (res == std::string::npos || res * char_height >= main_font->h)
    {
        return 0;
    }
    const int char_width = char_widths.at(res);
    SDL_FRect src_rect{ 0.0f, (float) char_height * res, (float) char_width, (float) char_height };
    SDL_FRect target_rect{ (float) x, (float) y, (float) char_width * scale, (float) char_height * scale };
    int ret = SDL_RenderTexture(renderer, main_font, &src_rect, &target_rect);
    return char_width * scale;
}

int write_string(const std::string& str, int x, int y, int scale=1)
{
    int cur_x = x;
    int cur_y = y;
    for (char c : str)
    {
        if (c == '\n')
        {
            cur_x = x;
            cur_y += 16 * scale;
        }
        else
        {
            cur_x += write_char(c, cur_x, cur_y, scale);
        }
    }
    return cur_y + 16 * scale;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer(PROJECT_NAME, 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    main_font = load_tex_from_bytes(read_bytes(std::filesystem::path(ASSETS_PATH "/graphics/fonts/mainfont.1bpp")), 16, 15, 1);

    SDL_StartTextInput(window);
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate)
{
    SDL_DestroyTexture(main_font);
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
        else if (keycode <= 0xFF && CHARMAP.find((char)keycode) != std::string::npos)
        {
            input += (char)keycode;
        }
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    int w = 0, h = 0;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_RenderClear(renderer);

    int y = write_string("Hello, world!", 1, 1, 2);
    write_string(input, 1, y, 2);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
