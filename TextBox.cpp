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
        font, GetInternalX(), GetInternalY(),
        label_text, x_scale, y_scale);
    RecalculateSize();
}

void TextBox::SetLabelText(const std::string& text)
{
    text_label->SetLabelText(text);
    RecalculateSize();
}

void TextBox::RecalculateSize()
{
    text_label->SetX(GetInternalX());
    text_label->SetY(GetInternalY());
    if(auto_expand)
    {
        auto [text_w, text_h] = text_label->GetExtent();
        text_w = std::max(init_width, text_w);
        text_h = std::max(init_height, text_h);
        if(text_w != GetInternalWidthTiles() || text_h != GetInternalHeightTiles())
        {
            SetInternalWidthPixels(text_w);
            SetInternalHeightPixels(text_h);
        }
    }
}

void TextBox::Draw(SDL_Renderer *renderer)
{
    RecalculateSize();
    UIBox::Draw(renderer);
    text_label->Draw(renderer);
}
