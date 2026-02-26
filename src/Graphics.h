#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL3/SDL.h>
#include "IDrawable.h"
#include <string>
#include <memory>
#include <vector>

class Texture;
class Font;

class Graphics
{
public:
    Graphics(unsigned int width, unsigned int height, const std::string& title);
    ~Graphics();

    void Render(const std::vector<std::shared_ptr<IDrawable>> drawables);
    
    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }
private:
    void Draw(const std::vector<std::shared_ptr<IDrawable>> drawables);
    
    unsigned int width;
    unsigned int height;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};

#endif // _GRAPHICS_H_
