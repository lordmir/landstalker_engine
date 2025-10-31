#ifndef _HUD_DEMO_H_
#define _HUD_DEMO_H_

#include "Hud.h"

class HudDemo : public Hud
{
public:
    HudDemo(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x = 0, unsigned int y = 0, unsigned int width = 38, unsigned int height = 1, float x_scale = 2.0f, float y_scale = 2.0f)
        : Hud(ui_tiles, palette, x, y, width, height, x_scale, y_scale)
    {}

    virtual void OnTick(unsigned int total_ticks) override
    {
        static double factor = 0.0;
        static bool direction = true;

        SetGold((GetGold() + 7) % (GOLD_CAP + 1));
        SetCurrentEkeeke(static_cast<unsigned int>(total_ticks / 60.0) % (EKEEKE_CAP + 1));

        if(total_ticks % 5 == 0)
        {
            if(GetCurrentHealth() >= GetMaxHealth())
            {
                SetCurrentHealth(0);
                SetMaxHealth((GetMaxHealth() + 1) % HEARTS_CAP);
            }
            else
            {
                SetCurrentHealth((GetCurrentHealth() + 1) % HEARTS_CAP);
            }
        }
        factor += 0.01f * (direction ? 1.0f : -1.0f);
        if(factor >= 1.0f)
        {
            factor = 1.0f;
            direction = false;
        }
        else if(factor <= 0.0f)
        {
            factor = 0.0f;
            direction = true;
        }

        SetMagicSwordCharge(factor);
    }
};

#endif // _HUD_DEMO_H_
