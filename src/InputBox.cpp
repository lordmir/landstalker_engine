#include "InputBox.h"

InputBox::InputBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette,
    std::shared_ptr<Font> font, const std::string& label_text,
    unsigned int x, unsigned int y,
    unsigned int width, unsigned int height, bool auto_expand,
    float x_scale, float y_scale)
    : TextBox(ui_tiles, palette, font, label_text, x, y, width, height, auto_expand, x_scale, y_scale)
{
    text_input = std::make_unique<TextInput>(
        font, static_cast<unsigned int>(x + ui_tiles->GetTileWidth() * x_scale),
        static_cast<unsigned int>(y + ui_tiles->GetTileHeight() * y_scale) +
        static_cast<unsigned int>(font->GetExtent(label_text, x_scale, y_scale).second),
        label_text, x_scale, y_scale);
}

void InputBox::OnKeyDown(SDL_Scancode scancode, SDL_Keymod mod)
{
    text_input->OnKeyDown(scancode, mod);
}

void InputBox::OnKeyUp(SDL_Scancode scancode, SDL_Keymod mod)
{
    text_input->OnKeyUp(scancode, mod);
}

void InputBox::Draw(SDL_Renderer* renderer)
{
    TextBox::SetLabelText(text_input->GetLabelText());
    TextBox::Draw(renderer);
}
