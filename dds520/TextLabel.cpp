#include "TextLabel.h"

TextLabel::TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale_x, float scale_y)
  : font(font), x(x), y(y), text(text), scale_x(scale_x), scale_y(scale_y), max_width(0), max_height(0), use_max_size(false), wrap(false)
{
    UpdateRenderText();
}

TextLabel::TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale)
  : font(font), x(x), y(y), text(text), scale_x(scale), scale_y(scale), max_width(0), max_height(0), use_max_size(false), wrap(false)
{
    UpdateRenderText();
}

void TextLabel::SetLabelText(const std::string& text_in)
{
    text = text_in;
    UpdateRenderText();
}

std::pair<unsigned int, unsigned int> TextLabel::GetMaxSize() const
{
    return {max_width, max_height};
}

void TextLabel::SetMaxSize(unsigned int max_width, unsigned int max_height)
{
    this->max_width = max_width;
    this->max_height = max_height;
    use_max_size = true;
    UpdateRenderText();
}

void TextLabel::ResetMaxSize()
{
    max_width = 0;
    max_height = 0;
    use_max_size = false;
    UpdateRenderText();
}

bool TextLabel::GetWrapEnabled() const
{
    return wrap;
}

void TextLabel::SetWrapEnabled(bool wrap)
{
    this->wrap = wrap;
    UpdateRenderText();
}

std::pair<unsigned int, unsigned int> TextLabel::GetExtent() const
{
    return font->GetExtent(render_text, scale_x, scale_y);
}

void TextLabel::Draw(SDL_Renderer* renderer)
{
    font->PrintString(renderer, x, y, render_text, scale_x, scale_y);
}

void TextLabel::UpdateRenderText()
{
    if (use_max_size)
    {
        if(wrap)
        {
            render_text = font->Wrap(text, max_width, max_height, scale_x, scale_y);
        }
        else
        {
            render_text = font->Constrain(text, max_width, max_height, scale_x, scale_y);
        }
    }
    else
    {
        render_text = text;
    }
}
