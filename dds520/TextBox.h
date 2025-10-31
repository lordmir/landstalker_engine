#ifndef _TEXT_BOX_H_
#define _TEXT_BOX_H_

#include "UIBox.h"
#include "TextLabel.h"
#include <utility>

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
    unsigned int GetCharWidth() const { return text_label->GetCharWidth(); }
    unsigned int GetCharHeight() const { return text_label->GetCharHeight(); }

    bool GetAutoExpand() const { return auto_expand; }
    void SetAutoExpand(bool auto_expand) { this->auto_expand = auto_expand; RecalculateSize(); }
    bool GetWrapEnabled() const { return text_label->GetWrapEnabled(); }
    void SetWrapEnabled(bool wrap) { text_label->SetWrapEnabled(wrap); }

    std::pair<unsigned int, unsigned int> GetMinSize() { return {min_width, min_height}; }
    std::pair<unsigned int, unsigned int> GetMaxSize() { return {max_width, max_height}; }
    void SetMinSize(unsigned int width, unsigned int height);
    void SetMaxSize(unsigned int width, unsigned int height);
    void ResetMaxSize();

    virtual void SetX(unsigned int x);
    virtual void SetY(unsigned int y);

    void RecalculateSize();
    virtual void Draw(SDL_Renderer* renderer) override;
private:
    bool auto_expand;
    unsigned int min_width;
    unsigned int min_height;
    unsigned int max_width;
    unsigned int max_height;
    bool use_max_size;
    std::unique_ptr<TextLabel> text_label;
};

#endif // _TEXT_BOX_H_
