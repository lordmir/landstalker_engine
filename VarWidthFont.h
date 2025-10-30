#ifndef _VAR_WIDTH_FONT_H_
#define _VAR_WIDTH_FONT_H_

#include "Font.h"

class VarWidthFont : public Font
{
public:
    VarWidthFont(std::shared_ptr<Texture> tileset, std::string charset,
        std::unordered_map<char, CONTROL_CHAR> control_chars,
        unsigned int space_width, unsigned int char_spacing, unsigned int line_spacing);

    virtual std::pair<unsigned int, unsigned int> GetExtent(char c, float x_scale, float y_scale) const override;
protected:
    virtual SDL_FRect GetSourceRect(int tile_index) const override;
    virtual SDL_FRect GetDestRect(int tile_index, float x, float y, float x_scale, float y_scale) const override;

private:
    std::vector<unsigned int> widths;
    unsigned int char_spacing;
    unsigned int line_spacing;
};

#endif // _VAR_WIDTH_FONT_H_
