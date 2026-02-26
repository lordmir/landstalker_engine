#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include "Drawable.h"
#include "Font.h"
#include <string>
#include <memory>
#include <cmath>

class TextLabel : public Drawable
{
public:
    TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale_x, float scale_y);
    TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale = 1.0f);
    virtual ~TextLabel() = default;
    virtual void SetLabelText(const std::string& text);
    const std::string& GetLabelText() const { return text; }
    std::pair<unsigned int, unsigned int> GetExtent() const;

    unsigned int GetX() const { return x; }
    unsigned int GetY() const { return y; }
    void SetX(unsigned int x) { this->x = x; }
    void SetY(unsigned int y) { this->y = y; }
    std::pair<unsigned int, unsigned int> GetMaxSize() const;
    void SetMaxSize(unsigned int max_width, unsigned int max_height);
    void ResetMaxSize();
    bool GetWrapEnabled() const;
    void SetWrapEnabled(bool wrap);

    unsigned int GetCharWidth() const { return static_cast<unsigned int>(ceil(font->GetCharWidth() * scale_x)); }
    unsigned int GetCharHeight() const { return static_cast<unsigned int>(ceil(font->GetCharHeight() * scale_y)); }

    virtual void Draw(SDL_Renderer* renderer) override;
protected:
    void UpdateRenderText();
    std::shared_ptr<Font> font;
private:
    std::string text;
    std::string render_text;
    unsigned int x;
    unsigned int y;
    unsigned int max_width;
    unsigned int max_height;
    bool use_max_size;
    bool wrap;
    float scale_x;
    float scale_y;
};

#endif // _TEXTLABEL_H_
