#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <SDL3/SDL.h>
#include "Tileset.h"
#include "Palette.h"
#include "Font.h"
#include "Graphics.h"
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <cstdint>

class Bitmap
{
public:
    Bitmap(unsigned int width, unsigned int height);
    bool InsertTile(std::shared_ptr<Tileset> tileset, const Tileset::Tile& tile, int x, int y, std::optional<std::shared_ptr<Palette>> = std::nullopt, std::vector<unsigned int> palette_idxs = {}, bool use_alpha = false);
    bool InsertChar(std::shared_ptr<Font> font, char c, int x, int y, std::optional<std::shared_ptr<Palette>> = std::nullopt, std::vector<unsigned int> palette_idxs = {}, bool use_alpha = true);
    bool InsertString(std::shared_ptr<Font> font, int x, int y, const std::string& s, std::optional<std::shared_ptr<Palette>> = std::nullopt, std::vector<unsigned int> palette_idxs = {}, bool use_alpha = true);
    
    void Clear(const SDL_Color& c = {0x00, 0x00, 0x00, 0xFF});
    void Resize(unsigned int new_width, unsigned int new_height);
    void Scroll(int x_amount, int y_amount, bool wraparound = false);

    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> GetSurface() const;
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> GetTexture(std::shared_ptr<Graphics> graphics) const;
private:
    std::vector<uint8_t> pixels;
    unsigned int width;
    unsigned int height;
};

#endif // _BITMAP_H_
