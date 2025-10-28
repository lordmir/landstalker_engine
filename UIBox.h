#ifndef _UI_BOX_H_
#define _UI_BOX_H_

#include <memory>
#include "Texture.h"
#include "Palette.h"
#include "IDrawable.h"


class UIBox : public IDrawable
{
public:
    enum class Tiles
    {
        EMPTY,
        TOP_CORNER,
        TOP_EDGE,
        VERTICAL_EDGE,
        EKEEKE_SYM_TOP,
        BOTTOM_CORNER,
        BOTTOM_EDGE,
        EKEEKE_SYM_BOTTOM,
        BODY,
        GOLD_SYM_BOTTOM,
        GOLD_SYM_TOP,
        DASH,
        C0_TOP,
        C1_TOP,
        C2_TOP,
        C3_TOP,
        C4_TOP,
        C5_TOP,
        C6_TOP,
        C7_TOP,
        C8_TOP,
        C9_TOP,
        SLASH_TOP,
        MEDIUM_HEART_TOP_LEFT_EMPTY,
        MEDIUM_HEART_TOP_LEFT_FULL,
        MEDIUM_HEART_TOP_OVERLAPPING_BOTH_EMPTY,
        MEDIUM_HEART_TOP_OVERLAPPING_BOTH_FULL,
        MEDIUM_HEART_TOP_OVERLAPPING_ONE_FULL,
        C0_BOTTOM,
        C1_BOTTOM,
        C2_BOTTOM,
        C3_BOTTOM,
        C4_BOTTOM,
        C5_BOTTOM,
        C6_BOTTOM,
        C7_BOTTOM,
        C8_BOTTOM,
        C9_BOTTOM,
        SLASH_BOTTOM,
        MEDIUM_HEART_BOTTOM_LEFT_EMPTY,
        MEDIUM_HEART_BOTTOM_LEFT_FULL,
        MEDIUM_HEART_BOTTOM_OVERLAPPING_BOTH_EMPTY,
        MEDIUM_HEART_BOTTOM_OVERLAPPING_BOTH_FULL,
        MEDIUM_HEART_BOTTOM_OVERLAPPING_ONE_FULL,
        LARGE_HEART_EMPTY_TOP_RIGHT,
        LARGE_HEART_FULL_TOP_RIGHT,
        LARGE_HEART_EMPTY_TOP_LEFT,
        LARGE_HEART_FULL_TOP_LEFT,
        SMALL_HEART_SINGLE_TOP_ROW_EMPTY,
        SMALL_HEART_SINGLE_TOP_ROW_FULL,
        SMALL_HEART_DOUBLE_TOP_ROW_EMPTY,
        SMALL_HEART_DOUBLE_TOP_ROW_FULL,
        SMALL_HEART_BOTTOM_ROW_EMPTY,
        SMALL_HEART_BOTTOM_ROW_FULL,
        SWORD_CHARGE_TIP_0,
        SWORD_CHARGE_TIP_1,
        SWORD_HILT_MIDDLE,
        SWORD_HILT_LEFT,
        SWORD_HILT_BOTTOM,
        SWORD_HILT_TOP,
        LARGE_HEART_EMPTY_BOTTOM_RIGHT,
        LARGE_HEART_FULL_BOTTOM_RIGHT,
        LARGE_HEART_EMPTY_BOTTOM_LEFT,
        LARGE_HEART_FULL_BOTTOM_LEFT,
        SWORD_CHARGE_BODY_0,
        SWORD_CHARGE_BODY_1,
        SWORD_CHARGE_BODY_2,
        SWORD_CHARGE_BODY_3,
        SWORD_CHARGE_BODY_4,
        SWORD_CHARGE_BODY_5,
        SWORD_CHARGE_BODY_6,
        SWORD_CHARGE_BODY_7,
        SWORD_CHARGE_TIP_2,
        TRANSPARENT
    };

    UIBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x = 0, unsigned int y = 0, unsigned int width = 3, unsigned int height = 3, float x_scale = 2.0f, float y_scale = 2.0f);
    unsigned int GetX() const { return x; }
    unsigned int GetY() const { return y; }
    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }
    float GetXScale() const { return x_scale; }
    float GetYScale() const { return y_scale; }
    SDL_Color GetBackgroundColour() const { return palette->GetSdlColour(box_background_idx); }

    void SetX(unsigned int nx) { x = nx; }
    void SetY(unsigned int ny) { y = ny; }
    void SetWidth(unsigned int w) { width = w; }
    void SetHeight(unsigned int h) { height = h; }
    void SetXScale(float xs) { x_scale = xs; }
    void SetYScale(float ys) { y_scale = ys; }
    void SetBackgroundColour(const SDL_Color& colour);

    virtual void Draw(SDL_Renderer* renderer) override;

protected:
    std::unique_ptr<Texture> ui_tiles;
    std::unique_ptr<Palette> palette;
private:
    const unsigned int box_background_idx = 10;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    float x_scale;
    float y_scale;
};

#endif // _UI_BOX_H_
