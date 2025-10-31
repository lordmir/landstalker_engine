#ifndef _TEXT_INPUT_H_
#define _TEXT_INPUT_H_

#include "TextLabel.h"
#include "IKeyInput.h"

class TextInput : public TextLabel, public IKeyInput
{
public:
    TextInput(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& label, float x_scale, float y_scale);
    TextInput(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& label, float scale = 1.0f);
    virtual ~TextInput() = default;

    virtual void SetLabelText(const std::string& text);
    void SetInputText(const std::string& text);
    const std::string& GetInputText() const;

    virtual void OnKeyDown(SDL_Scancode keycode, SDL_Keymod mod) override;
    virtual void OnKeyUp(SDL_Scancode keycode, SDL_Keymod mod) override;
    virtual void Draw(SDL_Renderer* renderer) override;

private:
    void UpdateLabel();

    std::string input_text;
    std::string label;
};

#endif // _TEXT_INPUT_H_
