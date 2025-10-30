#ifndef _HUD_H_
#define _HUD_H_

#include "UIBox.h"
#include <algorithm>

class Hud : public UIBox
{
public:
    Hud(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x = 0, unsigned int y = 0, unsigned int width = 38, unsigned int height = 1, float x_scale = 2.0f, float y_scale = 2.0f);
    virtual ~Hud() = default;

    void SetCurrentEkeeke(unsigned int ekeeke) { current_ekeeke = ekeeke; }
    unsigned int GetCurrentEkeeke() const { return current_ekeeke; }
    void SetMagicSwordCharge(double charge) { magic_sword_charge = charge; }
    double GetMagicSwordCharge() const { return magic_sword_charge; }
    void SetGold(unsigned int g) { gold = std::max(0U, g); }
    unsigned int GetGold() const { return gold; }
    void SetCurrentHealth(unsigned int health) { current_health = health; }
    unsigned int GetCurrentHealth() const { return current_health; }
    void SetMaxHealth(unsigned int health) { max_health = health; }
    unsigned int GetMaxHealth() const { return max_health; }

    static constexpr unsigned int GOLD_CAP = 99999;
    static constexpr unsigned int HEALTH_CAP = 200;
    static constexpr unsigned int HEARTS_CAP = 200;
    static constexpr unsigned int EKEEKE_CAP = 99;
    static constexpr unsigned int HP_PER_SMALL_HEART = 1;
    static constexpr unsigned int HP_PER_MEDIUM_HEART = 10;
    static constexpr unsigned int HP_PER_LARGE_HEART = 50;
protected:
    virtual void PrepareTilemap() override;
private:
    unsigned int GetGoldWidth() const;
    unsigned int GetEkeEkeWidth() const;
    unsigned int GetHealthDigitsWidth() const;
    unsigned int GetHealthSmallHeartsWidth() const;
    unsigned int GetHealthMediumHeartsWidth() const;
    unsigned int GetHealthLargeHeartsWidth() const;
    unsigned int GetTotalHeartWidth() const;

    unsigned int RenderSwordCharge(unsigned int col, unsigned int length);
    unsigned int RenderHealthDigits(unsigned int col);
    unsigned int RenderHealthHearts(unsigned int col);
    unsigned int RenderHealthSmallHearts(unsigned int col);
    unsigned int RenderHealthMediumHearts(unsigned int col);
    unsigned int RenderHealthLargeHearts(unsigned int col);
    unsigned int RenderEkeEke(unsigned int col);
    unsigned int RenderGold(unsigned int col);

    static constexpr unsigned int GOLD_RIGHT_PADDING = 0;
    static constexpr unsigned int EKEEKE_RIGHT_PADDING = 1;
    static constexpr unsigned int HEALTH_DIGITS_RIGHT_PADDING = 1;
    static constexpr unsigned int HEARTS_RIGHT_PADDING = 0;
    static constexpr unsigned int SWORD_CHARGE_RIGHT_PADDING = 1;
    static constexpr unsigned int SWORD_CHARGE_LEFT_PADDING = 1;
    static constexpr unsigned int MIN_SWORD_CHARGE_LENGTH = 4;

    unsigned int current_ekeeke = 0;
    double magic_sword_charge = 0.0;
    unsigned int gold = 0;
    unsigned int current_health = 0;
    unsigned int max_health = 0;
};

#endif // _HUD_H_