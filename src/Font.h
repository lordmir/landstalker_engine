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
        INVALID,
        PRINTABLE,
        STRING_START = 0x55,
        SELECTION_POINT = 0x56,
        NEWLINE = 0x57,
        GET_YES_OR_NO = 0x58,
        PAUSE_1S_NOSKIP = 0x59,
        NUMERIC_VAR = 0x5A,
        SPEAKER_NAME = 0x5B,
        STRING_END = 0x5E,
        ITEM_NAME = 0x5F,
        STRING_VAR = 0x60,
        PROMPT = 0x62,
        NEWLINE_AND_PROMPT = 0x63,
        PAUSE_1S = 0x64,
        PAUSE_1_5S = 0x65,
        PAUSE_2S = 0x66,
        SET_COLOUR = 0x68,
        BREAKING_HYPHEN = 0x69,
        LINE_BREAK_HINT = 0x6A,
        HYPHENATION_HINT = 0x6B
    };

    Font(std::shared_ptr<Texture> tileset, std::string charset, std::unordered_map<char, CONTROL_CHAR> control_chars);

    void SetColours(const std::vector<SDL_Color>& colours, const std::vector<int>& indices = {});

    virtual std::pair<unsigned int, unsigned int> GetExtent(char c, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> GetExtent(char c, float scale = 1.0f) const;
    virtual std::pair<unsigned int, unsigned int> GetExtent(std::string str, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> GetExtent(std::string str, float scale = 1.0f) const;

    std::pair<unsigned int, unsigned int> PrintCharacter(SDL_Renderer* renderer, unsigned int x, unsigned int y, char c, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> PrintCharacter(SDL_Renderer* renderer, unsigned int x, unsigned int y, char c, float scale = 1.0f) const;
    std::pair<unsigned int, unsigned int> PrintString(SDL_Renderer* renderer, unsigned int x, unsigned int y, std::string str, float x_scale, float y_scale) const;
    std::pair<unsigned int, unsigned int> PrintString(SDL_Renderer* renderer, unsigned int x, unsigned int y, std::string str, float scale = 1.0f) const;

    std::vector<std::string> Split(const std::string& input, const std::string& delimiters = " -.\n", bool strip_spaces = false) const;
    std::string Wrap(const std::string& input, unsigned int max_width, unsigned int max_height, double x_scale, double y_scale, const std::string& breaking_chars = " -.\n") const;
    std::string Constrain(const std::string& input, unsigned int max_width, unsigned int max_height, double x_scale, double y_scale) const;

    bool IsValid(char c) const;
    virtual unsigned int GetCharHeight() const { return texture->GetTileHeight(); }
    virtual unsigned int GetCharWidth() const { return texture->GetTileWidth(); }
    const std::string& GetCharset() const { return charset; }
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
