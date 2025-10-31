#include "Palette.h"
#include "Utils.h"
#include <algorithm>

// Definitions for the static palettes (simple example values)
const std::array<SDL_Color, 16> Palette::TEST_PALETTE =
{
    SDL_Color{ 0x00, 0x00, 0x00, 0xFF },
    SDL_Color{ 0xFF, 0x00, 0x00, 0x00 },
    SDL_Color{ 0x00, 0xFF, 0x00, 0x00 },
    SDL_Color{ 0x00, 0x00, 0xFF, 0x00 },
    SDL_Color{ 0xFF, 0xFF, 0x00, 0x00 },
    SDL_Color{ 0xFF, 0x00, 0xFF, 0x00 },
    SDL_Color{ 0x00, 0xFF, 0xFF, 0x00 },
    SDL_Color{ 0xC0, 0xC0, 0xC0, 0x00 },
    SDL_Color{ 0x80, 0x80, 0x80, 0x00 },
    SDL_Color{ 0x80, 0x00, 0x00, 0x00 },
    SDL_Color{ 0x00, 0x80, 0x00, 0x00 },
    SDL_Color{ 0x00, 0x00, 0x80, 0x00 },
    SDL_Color{ 0x80, 0x80, 0x00, 0x00 },
    SDL_Color{ 0x80, 0x00, 0x80, 0x00 },
    SDL_Color{ 0x00, 0x80, 0x80, 0x00 },
    SDL_Color{ 0xFF, 0xFF, 0xFF, 0x00 }
};

// Default values for rooms and sprites
const std::array<SDL_Color, 16> Palette::DEFAULT_COLOURS =
{
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0CCC),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000),
    Palette::GenesisToSdlColour(0x0000)
};


Palette::Palette(const std::filesystem::path& path, unsigned int begin, bool use_default_colours)
{
    auto palette = unpack_vec<uint16_t>(read_bytes(path));
    colours.resize(palette.size() + begin);
    if(use_default_colours)
    {
        colours.assign(DEFAULT_COLOURS.begin(), DEFAULT_COLOURS.end());
    }
    std::transform(palette.cbegin(), palette.cend(), colours.begin() + begin, GenesisToSdlColour);
    if(!colours.empty() && use_default_colours)
    {
        colours[0].a = 0x00;
    }
    init_colours = colours;
}

Palette::Palette(const std::vector<SDL_Color>& src, unsigned int begin, bool use_default_colours)
{
    colours.resize(src.size() + begin);
    if(use_default_colours)
    {
        colours.assign(DEFAULT_COLOURS.begin(), DEFAULT_COLOURS.end());
    }
    std::copy(src.cbegin(), src.cend(), colours.begin() + begin);
    if(!colours.empty() && use_default_colours)
    {
        colours[0].a = 0x00;
    }
    init_colours = colours;
}

Palette::Palette(const std::vector<uint16_t>& src, unsigned int begin, bool use_default_colours)
{
    colours.resize(src.size() + begin);
    if(use_default_colours)
    {
        colours.assign(DEFAULT_COLOURS.begin(), DEFAULT_COLOURS.end());
    }
    std::transform(src.cbegin(), src.cend(), colours.begin() + begin, GenesisToSdlColour);
    if(!colours.empty() && use_default_colours)
    {
        colours[0].a = 0x00;
    }
    init_colours = colours;
}

Palette::Palette(unsigned int size)
{
    colours.assign(TEST_PALETTE.begin(), TEST_PALETTE.end());
    colours.resize(size);
    if(!colours.empty())
    {
        colours[0].a = 0x00;
    }
    init_colours = colours;
}

const std::vector<SDL_Color>& Palette::GetSdlColours() const
{
    return colours;
}

std::vector<uint16_t> Palette::GetGenesisColours() const
{
    std::vector<uint16_t> out;
    out.reserve(colours.size());
    for (const auto& c : colours)
    {
        out.push_back(SdlColourToGenesis(c));
    }
    return out;
}

std::size_t Palette::GetColourCount() const
{
    return colours.size();
}

void Palette::Fade(const SDL_Color& target_colour, float factor)
{
    for(int i = 0; i < colours.size(); ++i)
    {
        colours[i].r = std::clamp<uint8_t>(init_colours[i].r + factor * target_colour.r, 0x00, 0xFF);
        colours[i].g = std::clamp<uint8_t>(init_colours[i].g + factor * target_colour.g, 0x00, 0xFF);
        colours[i].b = std::clamp<uint8_t>(init_colours[i].b + factor * target_colour.b, 0x00, 0xFF);
    }
}

void Palette::Reset()
{
    colours = init_colours;
}

SDL_Color Palette::GetSdlColour(unsigned int idx) const
{
    if (idx < colours.size())
    {
        return colours[idx];
    }
    return SDL_Color{0, 0, 0, 0};
}

uint16_t Palette::GetGenesisColour(unsigned int idx) const
{
    if (idx < colours.size())
    {
        return SdlColourToGenesis(colours[idx]);
    }
    return 0;
}

void Palette::SetSdlColour(unsigned int idx, SDL_Color colour)
{
    if (idx < colours.size())
    {
        colours[idx] = colour;
    }
}

void Palette::SetGenesisColour(unsigned int idx, uint16_t colour)
{
    if (idx < colours.size())
    {
        colours[idx] = GenesisToSdlColour(colour);
    }
}
