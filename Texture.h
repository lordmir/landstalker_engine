#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL3/SDL.h>
#include <filesystem>
#include <vector>
#include "Graphics.h"

class Texture
{
public:
    Texture(SDL_Renderer* renderer, const std::filesystem::path& path, unsigned int tile_w, unsigned int tile_h, unsigned int bitdepth);
    virtual ~Texture();

    void SetColours(const std::vector<SDL_Color>& colours, const std::vector<int>& indices = {});

    SDL_Texture* GetSdlTexture();    
protected:
    const unsigned int tile_x;
    const unsigned int tile_y;
    const unsigned int bitdepth;
    const unsigned int pixels_per_byte;
    const unsigned int bytes_per_tile;
    const unsigned int stride;
    const unsigned int total_colours;
    
    unsigned int tile_count;
    unsigned int total_h;

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* tex = nullptr;
    SDL_Surface* surf = nullptr;
};

#endif // _TEXTURE_H_
