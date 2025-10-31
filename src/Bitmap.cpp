#include "Bitmap.h"

Bitmap::Bitmap(unsigned int width, unsigned int height)
: width(width), height(height)
{
    pixels.resize(width * height * 4, 0);
}

bool Bitmap::InsertTile(std::shared_ptr<Tileset> tileset, const Tileset::Tile& tile, int x, int y, std::optional<std::shared_ptr<Palette>> palette_opt, std::vector<unsigned int> palette_idxs, bool use_alpha)
{
    // Implementation omitted for brevity
    return true;
}

bool Bitmap::InsertChar(std::shared_ptr<Font> font, char c, int x, int y, std::optional<std::shared_ptr<Palette>> palette_opt, std::vector<unsigned int> palette_idxs, bool use_alpha)
{
    // Implementation omitted for brevity
    return true;
}

bool Bitmap::InsertString(std::shared_ptr<Font> font, int x, int y, const std::string& s, std::optional<std::shared_ptr<Palette>> palette_opt, std::vector<unsigned int> palette_idxs, bool use_alpha)
{
    // Implementation omitted for brevity
    return true;
}

void Bitmap::Clear(const SDL_Color& c)
{
    size_t idx = 0;
    for(unsigned int y = 0; y < height; ++y)
    {
        for(unsigned int x = 0; x < width; ++x)
        {
            pixels[idx++] = c.r;
            pixels[idx++] = c.g;
            pixels[idx++] = c.b;
            pixels[idx++] = c.a;
        }
    }
}

void Bitmap::Resize(unsigned int new_width, unsigned int new_height)
{
    std::vector<uint8_t> new_pixels(new_width * new_height * 4, 0);
    unsigned int copy_width = std::min(width, new_width);
    unsigned int copy_height = std::min(height, new_height);

    size_t dst_idx = 0;
    size_t src_idx = 0;
    for(unsigned int y = 0; y < copy_height; ++y)
    {
        for(unsigned int i = 0; i < copy_width * 4; ++i)
        {
            new_pixels[dst_idx++] = pixels[src_idx++];
        }
        dst_idx += (new_width - copy_width) * 4;
    }

    pixels = std::move(new_pixels);
    width = new_width;
    height = new_height;
}

void Bitmap::Scroll(int x_amount, int y_amount, bool wraparound)
{
    std::vector<uint8_t> new_pixels(width * height * 4, 0);

    for(unsigned int y = 0; y < height; ++y)
    {
        for(unsigned int x = 0; x < width; ++x)
        {
            int src_x = x - x_amount;
            int src_y = y - y_amount;

            if(wraparound)
            {
                src_x = (src_x + width) % width;
                src_y = (src_y + height) % height;
            }

            if(src_x >= 0 && src_x < static_cast<int>(width) && src_y >= 0 && src_y < static_cast<int>(height))
            {
                size_t src_idx = (src_y * width + src_x) * 4;
                size_t dst_idx = (y * width + x) * 4;
                new_pixels[dst_idx] = pixels[src_idx];
                new_pixels[dst_idx + 1] = pixels[src_idx + 1];
                new_pixels[dst_idx + 2] = pixels[src_idx + 2];
                new_pixels[dst_idx + 3] = pixels[src_idx + 3];
            }
        }
    }
    pixels = std::move(new_pixels);
}

std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> Bitmap::GetSurface() const
{
    SDL_Surface* surf = SDL_CreateSurfaceFrom(
        width,
        height,
        SDL_PIXELFORMAT_RGBA32,
        const_cast<uint8_t*>(pixels.data()),
        width * 4
    );
    return std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(surf, SDL_DestroySurface);
}

std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> Bitmap::GetTexture(std::shared_ptr<Graphics> graphics) const
{
    auto surf = GetSurface();
    SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics->GetRenderer(), surf.get());
    return std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(tex, SDL_DestroyTexture);
}
