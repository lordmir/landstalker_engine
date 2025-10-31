#include "Tileset.h"
#include "Utils.h"

Tileset::Tileset(const std::vector<uint8_t> pixels, unsigned int tile_width, unsigned int tile_height, unsigned int bitdepth)
    : pixels(pixels)
    , tile_width(tile_width)
    , tile_height(tile_height)
    , bitdepth(bitdepth)
{
    unsigned int pixels_per_byte = 8 / bitdepth;
    unsigned int stride = tile_width / pixels_per_byte;
    unsigned int total_h = pixels.size() / stride;
    tile_count = total_h / tile_height;
}

Tileset::Tileset(const std::filesystem::path& path, unsigned int tile_width, unsigned int tile_height, unsigned int bitdepth)
    : tile_width(tile_width)
    , tile_height(tile_height)
    , bitdepth(bitdepth)
{
    unsigned int pixels_per_byte = 8 / bitdepth;
    unsigned int stride = tile_width / pixels_per_byte;
    unsigned int total_h = pixels.size() / stride;
    tile_count = total_h / tile_height;

    pixels = read_bytes(path);
}

Tileset::Tileset(unsigned int tile_width, unsigned int tile_height, unsigned int bitdepth, unsigned int tile_count)
    : Tileset(std::vector<uint8_t>(tile_count * tile_width * tile_height * bitdepth / 8), tile_width, tile_height, bitdepth)
{
}

std::shared_ptr<Tileset> Tileset::CreateFromPng(const std::filesystem::path& path, unsigned int tile_width, unsigned int tile_height, unsigned int bpp,
    std::optional<std::shared_ptr<Palette>> palette, std::optional<unsigned int> tile_count)
{
    // Implementation to create a Tileset from a PNG file
    // Implementation omitted for brevity
    return std::make_shared<Tileset>(path, tile_width, tile_height, bpp, tile_count.value_or(1));
}
