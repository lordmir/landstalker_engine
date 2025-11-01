#include "TextInput.h"

TextInput::TextInput(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& label, float x_scale, float y_scale)
: TextLabel(font, x, y, "", x_scale, y_scale),
  label(label)
{
    UpdateLabel();
}

TextInput::TextInput(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& label, float scale)
: TextLabel(font, x, y, "", scale, scale),
  label(label)
{
    UpdateLabel();
}

void TextInput::SetInputText(const std::string& text)
{
    input_text = text;
    UpdateLabel();
}

void TextInput::SetLabelText(const std::string& text)
{
    label = text;
    UpdateLabel();
}

const std::string& TextInput::GetInputText() const
{
    return input_text;
}

void TextInput::OnKeyDown(SDL_Scancode scancode, SDL_Keymod mod)
{
    SDL_Keycode keycode = SDL_GetKeyFromScancode(scancode, mod, false);
    if (keycode == SDLK_BACKSPACE || keycode == SDLK_DELETE)
    {
        if(!input_text.empty())
        {
            input_text.pop_back();
            UpdateLabel();
        }
    }
    else if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER)
    {
        if(font->IsValid('\n'))
        {
            input_text += '\n';
            UpdateLabel();
        }
    }
    else if (keycode == SDLK_V && (mod & (SDL_KMOD_LCTRL | SDL_KMOD_RCTRL)))
    {
        input_text += SDL_GetClipboardText();
        UpdateLabel();
    }
    else if (keycode < 256 && font->IsValid(static_cast<char>(keycode)))
    {
        char c = static_cast<char>(keycode);
        input_text += c;
        UpdateLabel();
    }
}

void TextInput::OnKeyUp(SDL_Scancode scancode, SDL_Keymod mod)
{
    // Handle key up events for text input
}

void TextInput::Draw(SDL_Renderer* renderer)
{
    // Draw the text input field
    TextLabel::Draw(renderer);
}

void TextInput::UpdateLabel()
{
    TextLabel::SetLabelText(label + input_text);
}