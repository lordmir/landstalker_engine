#ifndef _TILESET_H_
#define _TILESET_H_

#include "Palette.h"
#include <vector>
#include <cstdint>
#include <filesystem>
#include <optional>

class Tileset
{
    friend class Bitmap;
public:
    struct Tile
    {
        Tile(int index = 0, bool vflip = false, bool hflip = false) : index(index), vflip(vflip), hflip(hflip) {}
        int index;
        bool vflip;
        bool hflip;
    };

    Tileset(const std::vector<uint8_t> pixels, unsigned int tile_width = 8, unsigned int tile_height = 8, unsigned int bitdepth = 4, std::optional<unsigned int> tile_count = std::nullopt);
    Tileset(const std::filesystem::path& path, unsigned int tile_width, unsigned int tile_height, unsigned int bitdepth, std::optional<unsigned int> tile_count = std::nullopt);
    Tileset(unsigned int tile_width = 8, unsigned int tile_height = 8, unsigned int bitdepth = 4, unsigned int tile_count = 1);

    static std::shared_ptr<Tileset> CreateFromPng(const std::filesystem::path& path, unsigned int tile_width, unsigned int tile_height, unsigned int bpp = 32,
        std::optional<std::shared_ptr<Palette>> palette = std::nullopt, std::optional<unsigned int> tile_count = std::nullopt);
private:

    std::vector<uint8_t> pixels;
    unsigned int tile_width;
    unsigned int tile_height;
    unsigned int tile_count;
    unsigned int bitdepth;
};

#endif // _TILESET_H_
