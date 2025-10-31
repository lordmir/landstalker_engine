#include "Font.h"
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <cmath>

Font::Font(std::shared_ptr<Texture> tileset, std::string charset, std::unordered_map<char, CONTROL_CHAR> control_chars)
    : texture(tileset)
    , charset(charset)
    , control_chars(control_chars)
{
}


void Font::SetColours(const std::vector<SDL_Color>& colours, const std::vector<int>& indices)
{
    texture->SetColours(colours, indices);
}

std::pair<unsigned int, unsigned int> Font::GetExtent(char c, float x_scale, float y_scale) const
{
    // Implementation for getting the extent of a character with scaling
    if (GetControlChar(c) == CONTROL_CHAR::PRINTABLE)
    {
        return {static_cast<unsigned int>(ceil(texture->GetTileWidth() * x_scale)),
            static_cast<unsigned int>(ceil(texture->GetTileHeight() * y_scale))};
    }
    return {0, 0};
}

std::pair<unsigned int, unsigned int> Font::GetExtent(char c, float scale) const
{
    // Implementation for getting the extent of a character with uniform scaling
    return GetExtent(c, scale, scale);
}

std::pair<unsigned int, unsigned int> Font::GetExtent(std::string str, float x_scale, float y_scale) const
{
    // Implementation for getting the extent of a string with scaling
    if(str.empty())
    {
        return {0, 0};
    }

    std::vector<std::string> lines = Split(str, "\n");

    std::vector<std::pair<unsigned int, unsigned int>> line_extents;
    for(const auto& line : lines)
    {
        unsigned int line_width = 0;
        unsigned int line_height = GetExtent(' ', x_scale, y_scale).second;
        for(const char c : line)
        {
            auto [char_w, char_h] = GetExtent(c, x_scale, y_scale);
            line_width += char_w;
            line_height = std::max(line_height, char_h);
        }
        line_extents.push_back({line_width, line_height});
    }
    
    return {
        std::max_element(line_extents.begin(), line_extents.end(),
            [](auto lhs, auto rhs) {
                return lhs.first < rhs.first;
            })->first,
        std::accumulate(line_extents.begin(), line_extents.end(), 0,
            [](int sum, auto elem) {
                return sum + elem.second;
            })
    };
}

std::pair<unsigned int, unsigned int> Font::GetExtent(std::string str, float scale) const
{
    // Implementation for getting the extent of a string with uniform scaling
    return GetExtent(str, scale, scale);
}

std::pair<unsigned int, unsigned int> Font::PrintCharacter(SDL_Renderer* renderer, unsigned int x, unsigned int y, char c, float x_scale, float y_scale) const
{
    auto extent = GetExtent(c, x_scale, y_scale);
    int tile_index = CharToTile(c);
    auto src_rect = GetSourceRect(tile_index);
    auto dest_rect = GetDestRect(tile_index, x, y, x_scale, y_scale);
    SDL_RenderTexture(renderer, texture->GetSdlTexture(), &src_rect, &dest_rect);
    return {extent.first + x, extent.second + y};
}

std::pair<unsigned int, unsigned int> Font::PrintCharacter(SDL_Renderer* renderer, unsigned int x, unsigned int y, char c, float scale) const
{
    return PrintCharacter(renderer, x, y, c, scale, scale);
}

std::pair<unsigned int, unsigned int> Font::PrintString(SDL_Renderer* renderer, unsigned int x, unsigned int y, std::string str, float x_scale, float y_scale) const
{
    int cur_x = x;
    int cur_y = y;
    unsigned int max_height = GetExtent(' ', x_scale, y_scale).second;
    int max_x = x;
    
    for(const auto c : str)
    {
        int tile = CharToTile(c);
        if (tile != -1)
        {
            auto extent = GetExtent(c, x_scale, y_scale);
            PrintCharacter(renderer, cur_x, cur_y, c, x_scale, y_scale);
            max_height = std::max(extent.second, max_height);
            cur_x += extent.first;
        }
        else if(GetControlChar(c) == CONTROL_CHAR::NEWLINE)
        {
            max_x = std::max(cur_x, max_x);
            cur_x = x;
            cur_y += max_height;
            max_height = GetExtent(' ', x_scale, y_scale).second;
        }
    }
    max_x = std::max(cur_x, max_x);
    cur_y += max_height;
    return {max_x, cur_y};
}

std::pair<unsigned int, unsigned int> Font::PrintString(SDL_Renderer* renderer, unsigned int x, unsigned int y, std::string str, float scale) const
{
    return PrintString(renderer, x, y, str, scale, scale);
}

std::vector<std::string> Font::Split(const std::string &input, const std::string &delimiters, bool keep_delims) const
{
    std::vector<std::string> words;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    
    while ((pos = input.find_first_of(delimiters, prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            std::string word = input.substr(prev, keep_delims ? pos - prev + 1 : pos - prev);
            if(!word.empty())
            {
                words.push_back(word);
            }
        }
        prev = pos + 1;
    }
    
    if (prev < input.length())
    {
        std::string word = input.substr(prev, pos - prev);
        if(!word.empty())
        {
            words.push_back(word);
        }
    }

    return words;
}

std::string Font::Wrap(const std::string &input, unsigned int max_width, unsigned int max_height, double x_scale, double y_scale, const std::string &breaking_chars) const
{
    const unsigned int MAX_LINES = floor(max_height / GetExtent(' ', x_scale, y_scale).second);
    auto lines = Split(input, "\n");
    lines.resize(std::min<unsigned int>(lines.size(), MAX_LINES));
    std::vector<std::vector<std::string>> words;
    std::vector<std::string> out_lines;
    std::transform(lines.begin(), lines.end(), std::back_inserter(words),
        [&](const auto& line)
        {
            return Split(line, breaking_chars, true);
        });
    
    for(const auto& word_line : words)
    {
        // Word wrap
        std::string out_line;
        unsigned int remaining_length = max_width;
        for(const auto& word : word_line)
        {
            unsigned int word_length = GetExtent(word, x_scale, y_scale).first;
            if(word_length < remaining_length)
            {
                out_line += word;
                remaining_length -= word_length;
            }
            else
            {
                out_lines.push_back(out_line);
                out_line = word;
                remaining_length = max_width - word_length;
            }
        }
        out_lines.push_back(out_line);
    }

    out_lines.resize(std::min<unsigned int>(out_lines.size(), MAX_LINES));
    std::string text;
    for(const auto line : out_lines)
    {
        text += line + "\n";
    }
    if(!text.empty())
    {
        text.pop_back();
    }
    return text;
}

std::string Font::Constrain(const std::string &input, unsigned int max_width, unsigned int max_height, double x_scale, double y_scale) const
{
    const unsigned int MAX_LINES = floor(max_height / GetExtent(' ', x_scale, y_scale).second);
    auto lines = Split(input, "\n");
    lines.resize(std::min<unsigned int>(lines.size(), MAX_LINES));
    for (auto& line : lines)
    {
        while(GetExtent(line, x_scale, y_scale).first > max_width)
        {
            line.pop_back();
        }
    }

    std::string text;
    for(const auto line : lines)
    {
        text += line + "\n";
    }
    if(!text.empty())
    {
        text.pop_back();
    }
    return text;
}

SDL_FRect Font::GetSourceRect(int tile_index) const
{
    return texture->GetSourceRect(tile_index);
}

SDL_FRect Font::GetDestRect(int tile_index, float x, float y, float x_scale, float y_scale) const
{
    return texture->GetDestRect(tile_index, x, y, x_scale, y_scale);
}

int Font::CharToTile(char c) const
{
    // Implementation for converting a character to a tile index
    auto char_index = charset.find(c);
    if(char_index != std::string::npos && char_index < texture->GetTileCount())
    {
        return static_cast<int>(char_index);
    }
    return -1;
}

bool Font::IsValid(char c) const
{
    // Implementation for checking if a character corresponds to a valid tile
    return GetControlChar(c) != CONTROL_CHAR::INVALID;
}

Font::CONTROL_CHAR Font::GetControlChar(char c) const
{
    // Implementation for getting the control character type of a character
    if (control_chars.find(c) != control_chars.end())
    {
        return control_chars.at(c);
    }
    else if (CharToTile(c) != -1)
    {
        return CONTROL_CHAR::PRINTABLE;
    }
    return CONTROL_CHAR::INVALID;
}

unsigned int Font::GetLineCount(std::string str) const
{
    // Implementation for counting the number of lines in a string
    unsigned int count = 1;
    for (char c : str)
    {
        if (GetControlChar(c) == CONTROL_CHAR::NEWLINE)
        {
            count++;
        }
    }
    return count;
}