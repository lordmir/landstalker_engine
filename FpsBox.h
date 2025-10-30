#ifndef _FPS_BOX_H_
#define _FPS_BOX_H_

#include "TextBox.h"
#include <SDL3/SDL_pixels.h>
#include <format>
#include <algorithm>
#include <cmath>

class FpsBox : public TextBox
{
public:
    FpsBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette,
        std::shared_ptr<Font> font,
        unsigned int x = 0, unsigned int y = 0,
        unsigned int width = 128, unsigned int height = 16,
        float x_scale = 2.0f, float y_scale = 2.0f)
    : TextBox(ui_tiles, palette, font, "", x, y, width, height, false, x_scale, y_scale), t(0.0)
    {
        original = GetBackgroundColour();
    }

    virtual void OnUpdate(double delta) override
    {
        t += delta;
        SetLabelText(std::format("{0:4d} FPS", static_cast<unsigned int>(1.0 / delta)));

        SDL_Color blended = {
            static_cast<Uint8>(std::clamp<int>(original.r + sin(t) * (target.r - original.r), 0, 255)),
            static_cast<Uint8>(std::clamp<int>(original.g + sin(t) * (target.g - original.g), 0, 255)),
            static_cast<Uint8>(std::clamp<int>(original.b + sin(t) * (target.b - original.b), 0, 255)),
            0xFF
        };

        SetBackgroundColour(blended);
    }
private:
    double t;
    SDL_Color original = {};
    SDL_Color target = {0xFF, 0x00, 0x00, 0xFF};
};

#endif // _FPS_BOX_H_
