#ifndef _TEXTLABEL_H_
#define _TEXTLABEL_H_

#include "IDrawable.h"
#include "Font.h"
#include <string>
#include <memory>

class TextLabel : public IDrawable
{
public:
    TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale_x, float scale_y);
    TextLabel(std::shared_ptr<Font> font, unsigned int x, unsigned int y, const std::string& text, float scale = 1.0f);
    virtual ~TextLabel() = default;
    virtual void SetLabelText(const std::string& text);
    const std::string& GetLabelText() const { return text; }
    std::pair<unsigned int, unsigned int> GetExtent() const;

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
