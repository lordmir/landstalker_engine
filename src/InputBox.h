#ifndef _INPUT_BOX_H_
#define _INPUT_BOX_H_

#include "TextBox.h"
#include "TextInput.h"
#include "IKeyInput.h"

class InputBox : public TextBox, public IKeyInput
{
public:
    InputBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette,
        std::shared_ptr<Font> font, const std::string& label_text,
        unsigned int x = 0, unsigned int y = 0,
        unsigned int width = 40, unsigned int height = 6, bool auto_expand = false,
        float x_scale = 2.0f, float y_scale = 2.0f);

    void SetLabelText(const std::string& text) { text_input->SetLabelText(text); }
    const std::string& GetLabelText() const { return text_input->GetLabelText(); }
    void SetInputText(const std::string& text) { text_input->SetInputText(text); }
    const std::string& GetInputText() const { return text_input->GetInputText(); }

    virtual void OnKeyDown(SDL_Scancode scancode, SDL_Keymod mod) override;
    virtual void OnKeyUp(SDL_Scancode scancode, SDL_Keymod mod) override;

    virtual void Draw(SDL_Renderer* renderer) override;
private:
    std::unique_ptr<TextInput> text_input;
};

#endif // _INPUT_BOX_H_
