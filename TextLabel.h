#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include "Drawable.h"
#include "Font.h"
#include <string>
#include <memory>

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

    virtual void Draw(SDL_Renderer* renderer) override;
protected:
    std::shared_ptr<Font> font;
private:
    std::string text;
    unsigned int x;
    unsigned int y;
    float scale_x;
    float scale_y;
};

#endif // _TEXTLABEL_H_
