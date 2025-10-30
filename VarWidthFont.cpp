#include "VarWidthFont.h"
#include <cmath>

static inline unsigned int trailing_zeroes(uint64_t n) {
    unsigned bits = 0;
    uint64_t x = n;

    if (x) {
        /* mask the 32 low order bits, add 32 and shift them out if they are all 0 */
        if (!(x & 0xFFFFFFFFULL)) { bits += 32; x >>= 32; }
        /* mask the 16 low order bits, add 16 and shift them out if they are all 0 */
        if (!(x & 0x0000FFFFULL)) { bits += 16; x >>= 16; }
        /* mask the 8 low order bits, add 8 and shift them out if they are all 0 */
        if (!(x & 0x000000FFULL)) { bits += 8; x >>= 8; }
        /* mask the 4 low order bits, add 4 and shift them out if they are all 0 */
        if (!(x & 0x0000000FULL)) { bits += 4; x >>= 4; }
        /* mask the 2 low order bits, add 2 and shift them out if they are all 0 */
        if (!(x & 0x00000003ULL)) { bits += 2; x >>= 2; }
        /* mask the low order bit and add 1 if it is 0 */
        bits += (x & 1ULL) ^ 1;
    }
    return bits;
}

VarWidthFont::VarWidthFont(std::shared_ptr<Texture> tileset, std::string charset,
    std::unordered_map<char, CONTROL_CHAR> control_chars,
    unsigned int space_width, unsigned int char_spacing, unsigned int line_spacing)
    : Font(tileset, charset, control_chars)
    , char_spacing(char_spacing), line_spacing(line_spacing)
{
    widths.assign(tileset->GetTileCount(), space_width);

    for(const auto c : charset)
    {
        int tile_index = CharToTile(c);
        if (c == ' ' || tile_index == -1)
        {
            continue;
        }
        const int line_begin = tile_index * tileset->GetRowStride() * tileset->GetTileHeight();
        const int line_end = (tile_index + 1) * tileset->GetRowStride() * tileset->GetTileHeight();
        const uint8_t* pixels = reinterpret_cast<uint8_t*>(tileset->GetSdlSurface()->pixels) + line_begin;
        unsigned int width = 0;
        for(int i = 0; i < tileset->GetTileHeight(); ++i)
        {
            uint64_t row = 0ULL;
            for(int j = 0; j < tileset->GetRowStride(); ++j)
            {
                row <<= 8;
                row |= *pixels++;
            }
            if(row == 0)
            {
                continue;
            }
            auto blank_pixels = trailing_zeroes(row) / tileset->GetTileBitdepth();
            unsigned int row_width = tileset->GetTileWidth() - blank_pixels;
            width = std::max(row_width, width);
        }
        widths[tile_index] = width;
    }
}

std::pair<unsigned int, unsigned int> VarWidthFont::GetExtent(char c, float x_scale, float y_scale) const
{
    int tile_index = CharToTile(c);
    if (tile_index != -1)
    {
        return {static_cast<unsigned int>(ceil((widths.at(tile_index) + char_spacing) * x_scale)),
            static_cast<unsigned int>(ceil(Font::GetExtent(c, x_scale, y_scale).second + line_spacing * y_scale))};
    }
    return {0, 0};
}

SDL_FRect VarWidthFont::GetSourceRect(int tile_index) const
{
    // Implementation for getting the source rectangle of a character
    if(tile_index != -1)
    {
        return SDL_FRect
        {
            0.0f,
            static_cast<float>(GetCharHeight() * tile_index),
            static_cast<float>(widths.at(tile_index)),
            static_cast<float>(GetCharHeight())
        };
    }
    return SDL_FRect{};
}

SDL_FRect VarWidthFont::GetDestRect(int tile_index, float x, float y, float x_scale, float y_scale) const
{
    if(tile_index != -1)
    {
        return SDL_FRect
        {
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(ceil(widths.at(tile_index) * x_scale)),
            static_cast<float>(ceil(GetCharHeight() * y_scale))
        };
    }
    return SDL_FRect{};
}
