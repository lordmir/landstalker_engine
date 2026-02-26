#ifndef _PALETTE_H_
#define _PALETTE_H_

#include <SDL3/SDL_pixels.h>
#include <filesystem>
#include <vector>
#include <array>
#include <cstdint>
#include <cstdlib>

class Palette
{
public:
    Palette(const std::filesystem::path& path, unsigned int begin = 0U, bool use_default_colours = false);
    Palette(const std::vector<SDL_Color>&, unsigned int begin = 0U, bool use_default_colours = false);
    Palette(const std::vector<uint16_t>&, unsigned int begin = 0U, bool use_default_colours = false);
    Palette(unsigned int size = 16U);

    const std::vector<SDL_Color>& GetSdlColours() const;
    std::vector<uint16_t> GetGenesisColours() const;
    std::size_t GetColourCount() const;

    void Fade(const SDL_Color& target_colour, float factor);
    void Reset();

    SDL_Color GetSdlColour(unsigned int idx) const;
    uint16_t GetGenesisColour(unsigned int idx) const;
    void SetSdlColour(unsigned int idx, SDL_Color colour);
    void SetGenesisColour(unsigned int idx, uint16_t colour);

    static constexpr SDL_Color GenesisToSdlColour(uint16_t gen_colour)
    {
        return SDL_Color
        {
            static_cast<uint8_t>((gen_colour & 0x000E) * 15),
            static_cast<uint8_t>(((gen_colour & 0x00E0) >> 4) * 15),
            static_cast<uint8_t>(((gen_colour & 0x0E00) >> 8) * 15),
            0xFF
        };
    }

    static constexpr uint16_t SdlColourToGenesis(SDL_Color sdl_colour)
    {
        return static_cast<uint16_t>
        (
            (((sdl_colour.b / 15) & 0x0E) << 8) |
            (((sdl_colour.g / 15) & 0x0E) << 4) |
            ((sdl_colour.r / 15) & 0x0E)
        );
    }
private:
    std::vector<SDL_Color> init_colours;
    std::vector<SDL_Color> colours;
    static const std::array<SDL_Color, 16> TEST_PALETTE;
    static const std::array<SDL_Color, 16> DEFAULT_COLOURS;
};

#endif // _PALETTE_H_
