#include "Texture.h"
#include "Utils.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <array>

static const std::array<SDL_PixelFormat, 9> PIXEL_FORMATS = {
    SDL_PIXELFORMAT_UNKNOWN, // 0
    SDL_PIXELFORMAT_INDEX1MSB, // 1
    SDL_PIXELFORMAT_INDEX2MSB, // 2
    SDL_PIXELFORMAT_UNKNOWN, // 3
    SDL_PIXELFORMAT_INDEX4MSB, // 4
    SDL_PIXELFORMAT_UNKNOWN, // 5
    SDL_PIXELFORMAT_UNKNOWN, // 6
    SDL_PIXELFORMAT_UNKNOWN, // 7
    SDL_PIXELFORMAT_INDEX8, // 8
};

static const std::vector<SDL_Color> DEFAULT_PALETTE = {
    {0x00, 0x00, 0x00, 0xFF},
    {0xFF, 0xFF, 0xFF, 0x00}
};

Texture::Texture(SDL_Renderer* renderer, const std::filesystem::path& path, unsigned int tile_w, unsigned int tile_h, unsigned int bitdepth)
	: tile_x(tile_x)
	, tile_y(tile_y)
	, bitdepth(bitdepth)
    , pixels_per_byte(8 / bitdepth)
    , bytes_per_tile(tile_w * tile_h / pixels_per_byte)
    , stride(tile_w / pixels_per_byte)
    , total_colours(1 << bitdepth)
	, tile_count(0)
    , renderer(renderer)
	, tex(nullptr)
	, surf(nullptr)
{
    auto bytes = read_bytes(path);

    total_h = bytes.size() / stride;
    tile_count = total_h / tile_h;

    surf = SDL_CreateSurfaceFrom(tile_w, total_h, PIXEL_FORMATS[bitdepth],
        bytes.data(), stride);
    
    SetColours(DEFAULT_PALETTE);
}

Texture::~Texture()
{
	// Free SDL resources if they were allocated
	if (tex) {
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}
	if (surf) {
		SDL_DestroySurface(surf);
		surf = nullptr;
	}
}

void Texture::SetColours(const std::vector<SDL_Color>& colours, const std::vector<int>& indices)
{
    auto pal = SDL_CreatePalette(total_colours);
    for (unsigned int i = 0; i < std::min<unsigned int>(colours.size(), total_colours); ++i)
    {
        unsigned int colour_index = i;
        if (i < indices.size())
        {
            colour_index = indices.at(i);
        }
        pal->colors[i] = colours.at(colour_index);
    }
    SDL_SetSurfacePalette(surf, pal);
	if (tex) {
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_PIXELART);
}

SDL_Texture* Texture::GetSdlTexture()
{
    return tex;
}
