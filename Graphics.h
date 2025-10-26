#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL3/SDL.h>
#include <string>

class Texture;
class Font;

class Graphics
{
public:
    Graphics(int width, int height, const std::string& title);
    ~Graphics();

    bool DrawTile(Texture& tex, int tile, int x, int y, int scale_x, int scale_y);
    bool DrawTile(Texture& tex, int tile, int x, int y, int scale = 1);

    int PutChar(Font& font, char chr, int x, int y, int scale = 1);
    int PutString(Font& font, const std::string& str, int x, int y, int scale = 1);

    void OnRender();

    const SDL_Renderer* GetRenderer() const;
    const SDL_Window* GetWindow() const;

private:
    int width;
    int height;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};

#endif // _GRAPHICS_H_
