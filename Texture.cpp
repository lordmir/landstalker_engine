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

Texture::Texture(std::shared_ptr<Graphics> graphics, const std::filesystem::path& path, unsigned int tile_w, unsigned int tile_h, unsigned int bitdepth)
	: tile_width(tile_w)
	, tile_height(tile_h)
	, bitdepth(bitdepth)
    , pixels_per_byte(8 / bitdepth)
    , bytes_per_tile(tile_w * tile_h / pixels_per_byte)
    , stride(tile_w / pixels_per_byte)
    , total_colours(1 << bitdepth)
	, tile_count(0)
    , graphics(graphics)
	, tex(nullptr)
	, surf(nullptr)
{
    bytes = std::make_unique<std::vector<uint8_t>>(read_bytes(path));

    total_h = bytes->size() / stride;
    tile_count = total_h / tile_h;

    surf = SDL_CreateSurfaceFrom(tile_w, total_h, PIXEL_FORMATS[bitdepth],
        bytes->data(), stride);
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);
    SetColours(DEFAULT_PALETTE);
}

Texture::Texture(const Texture &rhs)
    : tile_width(rhs.tile_width)
    , tile_height(rhs.tile_height)
    , bitdepth(rhs.bitdepth)
    , pixels_per_byte(8 / bitdepth)
    , bytes_per_tile(tile_width * tile_height / pixels_per_byte)
    , stride(tile_width / pixels_per_byte)
    , total_colours(1 << bitdepth)
	, tile_count(rhs.tile_count)
    , total_h(rhs.total_h)
    , graphics(rhs.graphics)
	, tex(nullptr)
	, surf(nullptr)
{
    bytes = std::make_unique<std::vector<uint8_t>>(*(rhs.bytes));
    surf = SDL_CreateSurfaceFrom(tile_width, total_h, PIXEL_FORMATS[bitdepth],
        bytes->data(), stride);
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);
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
        SDL_SetPaletteColors(pal, &colours.at(colour_index), i, 1);
    }
    if(!SDL_SetSurfacePalette(surf, pal))
    {
        throw std::runtime_error(std::string("Unable to set palette: ") + SDL_GetError());
    }
	if (tex) {
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}
    tex = SDL_CreateTextureFromSurface(graphics->GetRenderer(), surf);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_PIXELART);
}

SDL_Texture* Texture::GetSdlTexture()
{
    return tex;
}

SDL_Surface* Texture::GetSdlSurface()
{
    return surf;
}

SDL_FRect Texture::GetSourceRect(int tile_index) const
{
    // Implementation for getting the source rectangle of a character
    if(tile_index >= 0 && static_cast<unsigned int>(tile_index) < tile_count)
    {
        return SDL_FRect{0.0f, static_cast<float>(tile_height * tile_index),
                         static_cast<float>(tile_width), static_cast<float>(tile_height)};
    }
    return SDL_FRect{};
}

SDL_FRect Texture::GetDestRect(int tile_index, float x, float y, float x_scale, float y_scale) const
{
    // Implementation for getting the destination rectangle of a character with scaling
    if(tile_index >= 0 && static_cast<unsigned int>(tile_index) < tile_count)
    {
        return SDL_FRect{x, y, static_cast<float>(tile_width) * x_scale,
            static_cast<float>(tile_height) * y_scale};
    }
    return SDL_FRect{};
}

void Texture::DrawTile(SDL_Renderer *renderer, const Tile &tile, float x, float y, float x_scale, float y_scale)
{
    auto src_rect = GetSourceRect(tile.index);
    auto dest_rect = GetDestRect(tile.index, x, y, x_scale, y_scale);
    if (!tile.hflip && !tile.vflip)
    {
        SDL_RenderTexture(renderer, tex, &src_rect, &dest_rect);
    }
    else
    {
        SDL_FlipMode flip_mode = static_cast<SDL_FlipMode>((tile.hflip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (tile.vflip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));
        SDL_RenderTextureRotated(renderer, tex, &src_rect, &dest_rect, 0.0, nullptr, flip_mode);
    }
}
