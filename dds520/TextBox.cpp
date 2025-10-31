#include "TextBox.h"

TextBox::TextBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette,
    std::shared_ptr<Font> font, const std::string& label_text,
    unsigned int x, unsigned int y,
    unsigned int width, unsigned int height, bool auto_expand,
    float x_scale, float y_scale)
    : UIBox(ui_tiles, palette, x, y, width, height, x_scale, y_scale)
    , min_width(width), min_height(height), use_max_size(!auto_expand), auto_expand(auto_expand)
    
{
    text_label = std::make_unique<TextLabel>(
        font, GetInternalX(), GetInternalY(),
        label_text, x_scale, y_scale);
    ResetMaxSize();
}

void TextBox::SetLabelText(const std::string& text)
{
    if(text != text_label->GetLabelText())
    {
        text_label->SetLabelText(text);
        RecalculateSize();
    }
}


void TextBox::SetMinSize(unsigned int width, unsigned int height)
{
    min_width = width;
    min_height = height;
    RecalculateSize();
}

void TextBox::SetMaxSize(unsigned int width, unsigned int height)
{
    max_width = width;
    max_height = height;
    use_max_size = true;
    text_label->SetMaxSize(max_width, max_height);
    RecalculateSize();
}

void TextBox::ResetMaxSize()
{
    max_width = 0;
    max_height = 0;
    use_max_size = false;
    text_label->ResetMaxSize();
    RecalculateSize();
}

void TextBox::SetX(unsigned int x)
{
    UIBox::SetX(x);
    RecalculateSize();
}

void TextBox::SetY(unsigned int y)
{
    UIBox::SetY(y);
    RecalculateSize();
}

void TextBox::RecalculateSize()
{
    text_label->SetX(GetInternalX());
    text_label->SetY(GetInternalY());
    if(auto_expand)
    {
        auto [text_w, text_h] = text_label->GetExtent();
        text_w = std::max(min_width, text_w);
        text_h = std::max(min_height, text_h);
        if(use_max_size && max_width >= min_width)
        {
            text_w = std::min(max_width, text_w);
        }
        if(use_max_size && max_height >= min_height)
        {
            text_h = std::min(max_height, text_h);
        }
        if(text_w != GetInternalWidthTiles() || text_h != GetInternalHeightTiles())
        {
            SetInternalWidthPixels(text_w);
            SetInternalHeightPixels(text_h);
        }
    }
}

void TextBox::Draw(SDL_Renderer *renderer)
{
    UIBox::Draw(renderer);
    text_label->Draw(renderer);
}
