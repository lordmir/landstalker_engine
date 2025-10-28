#include "TextBox.h"

TextBox::TextBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette,
    std::shared_ptr<Font> font, const std::string& label_text,
    unsigned int x, unsigned int y,
    unsigned int width, unsigned int height, bool auto_expand,
    float x_scale, float y_scale)
    : UIBox(ui_tiles, palette, x, y, width, height, x_scale, y_scale)
    , init_width(width), init_height(height), auto_expand(auto_expand)
    
{
    text_label = std::make_unique<TextLabel>(
        font, static_cast<unsigned int>(x + ui_tiles->GetTileWidth() * x_scale),
        static_cast<unsigned int>(y + ui_tiles->GetTileHeight() * y_scale),
        label_text, x_scale, y_scale);
}

void TextBox::RecalculateSize()
{
    if(auto_expand)
    {
        auto [text_w, text_h] = text_label->GetExtent();
        unsigned int tile_width = static_cast<unsigned int>(ui_tiles->GetTileWidth() * GetXScale());
        unsigned int tile_height = static_cast<unsigned int>(ui_tiles->GetTileHeight() * GetYScale());
        unsigned int new_width = std::max(init_width, (text_w + tile_width - 1) / tile_width);
        unsigned int new_height = std::max(init_height, (text_h + tile_height - 1) / tile_height);
        if(new_width != GetWidth() || new_height != GetHeight())
        {
            SetWidth(new_width);
            SetHeight(new_height);
        }
    }
}

void TextBox::Draw(SDL_Renderer *renderer)
{
    RecalculateSize();
    UIBox::Draw(renderer);
    text_label->Draw(renderer);
}
