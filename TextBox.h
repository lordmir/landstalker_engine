#ifndef _TEXT_BOX_H_
#define _TEXT_BOX_H_

#include "UIBox.h"
#include "TextLabel.h"

class TextBox : public UIBox
{
public:
    TextBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette,
        std::shared_ptr<Font> font, const std::string& label_text,
        unsigned int x = 0, unsigned int y = 0,
        unsigned int width = 320, unsigned int height = 32, bool auto_expand = false,
        float x_scale = 2.0f, float y_scale = 2.0f);

    void SetLabelText(const std::string& text);
    const std::string& GetLabelText() const { return text_label->GetLabelText(); }

    void RecalculateSize();
    virtual void Draw(SDL_Renderer* renderer) override;
private:
    bool auto_expand;
    unsigned int init_width;
    unsigned int init_height;
    std::unique_ptr<TextLabel> text_label;
};

#endif // _TEXT_BOX_H_
