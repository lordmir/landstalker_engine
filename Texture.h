#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL3/SDL.h>
#include <filesystem>
#include <vector>
#include <memory>
#include "Graphics.h"

class Texture
{
public:
    Texture(std::shared_ptr<Graphics> graphics, const std::filesystem::path& path, unsigned int tile_w, unsigned int tile_h, unsigned int bitdepth);
    virtual ~Texture();

    void SetColours(const std::vector<SDL_Color>& colours, const std::vector<int>& indices = {});

    SDL_Texture* GetSdlTexture();
    SDL_Surface* GetSdlSurface();
    unsigned int GetTileWidth() const { return tile_width; }
    unsigned int GetTileHeight() const { return tile_height; }
    unsigned int GetTileCount() const { return tile_count; }
    unsigned int GetTileBitdepth() const { return bitdepth; }
    unsigned int GetRowStride() const { return stride; }

protected:
    const unsigned int tile_width;
    const unsigned int tile_height;
    const unsigned int bitdepth;
    const unsigned int pixels_per_byte;
    const unsigned int bytes_per_tile;
    const unsigned int stride;
    const unsigned int total_colours;
    
    unsigned int tile_count;
    unsigned int total_h;

    std::unique_ptr<std::vector<uint8_t>> bytes;
    std::shared_ptr<Graphics> graphics;
    SDL_Texture* tex = nullptr;
    SDL_Surface* surf = nullptr;
};

#endif // _TEXTURE_H_
