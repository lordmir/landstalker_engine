#include "TextLabel.h"

TextLabel::TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale_x, float scale_y)
  : font(font), x(x), y(y), text(text), scale_x(scale_x), scale_y(scale_y)
{}

TextLabel::TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale)
  : font(font), x(x), y(y), text(text), scale_x(scale), scale_y(scale)
{}

void TextLabel::SetLabelText(const std::string& text_in)
{
    text = text_in;
}

void TextLabel::Draw(SDL_Renderer* renderer)
{
    font->PrintString(renderer, x, y, text, scale_x, scale_y);
}
