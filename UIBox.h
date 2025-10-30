#ifndef _UI_BOX_H_
#define _UI_BOX_H_

#include <memory>
#include "Texture.h"
#include "Palette.h"
#include "Drawable.h"


class UIBox : public Drawable
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
        SWORD_CHARGE_BODY_8,
        SWORD_CHARGE_BODY_9,
        SWORD_CHARGE_TIP_2,
        TRANSPARENT
    };

    UIBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x = 0, unsigned int y = 0, unsigned int width = 24, unsigned int height = 24, float x_scale = 2.0f, float y_scale = 2.0f);
    unsigned int GetX() const;
    unsigned int GetY() const;
    unsigned int GetInternalX() const;
    unsigned int GetInternalY() const;
    unsigned int GetInternalWidthPixels() const;
    unsigned int GetInternalHeightPixels() const;
    unsigned int GetInternalWidthTiles() const;
    unsigned int GetInternalHeightTiles() const;
    unsigned int GetExternalWidthPixels() const;
    unsigned int GetExternalHeightPixels() const;
    unsigned int GetExternalWidthTiles() const;
    unsigned int GetExternalHeightTiles() const;
    float GetXScale() const;
    float GetYScale() const;
    SDL_Color GetBackgroundColour() const;

    void SetX(unsigned int nx);
    void SetY(unsigned int ny);
    void SetInternalWidthPixels(unsigned int w);
    void SetInternalHeightPixels(unsigned int h);
    void SetInternalWidthTiles(unsigned int w);
    void SetInternalHeightTiles(unsigned int h);
    void SetXScale(float xs);
    void SetYScale(float ys);
    void SetScale(float xs, float ys);
    void SetScale(float s);
    void SetBackgroundColour(const SDL_Color& colour);
    void SetTile(const Texture::Tile& tile, const unsigned int x, const unsigned int y);
    Texture::Tile GetTile(const unsigned int x, const unsigned int y) const;
    Texture::Tile& Tile(const unsigned int x, const unsigned int y);

    virtual void Draw(SDL_Renderer* renderer) override;
    
    unsigned int tile_width;
    unsigned int tile_height;
protected:
    virtual void PrepareTilemap();
    std::unique_ptr<Texture> ui_tiles;
    std::unique_ptr<Palette> palette;
    std::vector<Texture::Tile> tiles;
private:
    const unsigned int box_background_idx = 10;
    const unsigned int border_thickness = 1;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    float x_scale;
    float y_scale;
};

#endif // _UI_BOX_H_
