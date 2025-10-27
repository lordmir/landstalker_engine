#ifndef _FONT_H_
#define _FONT_H_

#include "Texture.h"
#include <string>
#include <unordered_map>

class Font
{
public:
    enum class CONTROL_CHAR
    {
        PRINTABLE,
        INVALID,
        NEWLINE,
        STRING_START,
        STRING_END,
        PROMPT_TO_CONTINUE,
        GET_YES_OR_NO,
        PAUSE_1S,
        LINE_BREAK_HINT,
        HYPHENATION_HINT,
        BREAKING_HYPHEN
    };

    Font(std::shared_ptr<Texture> tileset, std::string charset, std::unordered_map<char, CONTROL_CHAR> control_chars);

    virtual std::pair<unsigned int, unsigned int> GetExtent(char c, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> GetExtent(char c, float scale = 1.0f) const;
    virtual std::pair<unsigned int, unsigned int> GetExtent(std::string str, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> GetExtent(std::string str, float scale = 1.0f) const;

    std::pair<unsigned int, unsigned int> PrintCharacter(SDL_Renderer* renderer, unsigned int x, unsigned int y, char c, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> PrintCharacter(SDL_Renderer* renderer, unsigned int x, unsigned int y, char c, float scale = 1.0f) const;
    std::pair<unsigned int, unsigned int> PrintString(SDL_Renderer* renderer, unsigned int x, unsigned int y, std::string str, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> PrintString(SDL_Renderer* renderer, unsigned int x, unsigned int y, std::string str, float scale = 1.0f) const;
    
    bool IsValid(char c) const;
    unsigned int GetCharHeight() const { return texture->GetTileHeight(); }
    unsigned int GetCharWidth() const { return texture->GetTileWidth(); }
protected:
    virtual SDL_FRect GetSourceRect(int tile_index) const;
    virtual SDL_FRect GetDestRect(int tile_index, float x, float y, float x_scale, float y_scale) const;

    int CharToTile(char c) const;
    CONTROL_CHAR GetControlChar(char c) const;
    unsigned int GetLineCount(std::string str) const;

private:
    std::shared_ptr<Texture> texture;
    const std::string charset;
    const std::unordered_map<char, CONTROL_CHAR> control_chars;
};

#endif // _FONT_H_
