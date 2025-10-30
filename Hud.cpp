#include "Hud.h"
#include <cassert>
#include <format>

static constexpr unsigned int GetHealthSmallHeartsCount(unsigned int health)
{
    return std::min(health, Hud::HP_PER_MEDIUM_HEART / Hud::HP_PER_SMALL_HEART);
}

static constexpr unsigned int GetHealthMediumHeartsCount(unsigned int health)
{
    return health <= Hud::HP_PER_MEDIUM_HEART ? 0U :
        std::min(health - 1, Hud::HP_PER_LARGE_HEART - Hud::HP_PER_MEDIUM_HEART) / Hud::HP_PER_MEDIUM_HEART;
}

static constexpr unsigned int GetHealthLargeHeartsCount(unsigned int health)
{
    return health <= Hud::HP_PER_LARGE_HEART ? 0U : (health - 1) / Hud::HP_PER_LARGE_HEART;
}

static constexpr unsigned int GetHealthSmallHeartsFilled(unsigned int health)
{
    return health == 0U ? 0U : 1 + (health - 1) % (Hud::HP_PER_MEDIUM_HEART / Hud::HP_PER_SMALL_HEART);
}

static constexpr unsigned int GetHealthMediumHeartsFilled(unsigned int health)
{
    const unsigned int medium_heart_health = (health - GetHealthSmallHeartsFilled(health) * Hud::HP_PER_SMALL_HEART) % Hud::HP_PER_LARGE_HEART;
    return health == 0U ? 0U : (medium_heart_health + Hud::HP_PER_MEDIUM_HEART - 1) / Hud::HP_PER_MEDIUM_HEART;
}

static constexpr unsigned int GetHealthLargeHeartsFilled(unsigned int health)
{
    return health == 0U ? 0U : (health - 1) / Hud::HP_PER_LARGE_HEART;
}

Hud::Hud(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x, unsigned int y, unsigned int width, unsigned int height, float x_scale, float y_scale)
    : UIBox(ui_tiles, palette, x, y, width, height, x_scale, y_scale),
      current_ekeeke(0),
      magic_sword_charge(0),
      gold(0),
      current_health(0),
      max_health(0)
{
}

void Hud::PrepareTilemap()
{
    assert(GetInternalWidthTiles() >= GetGoldWidth() + GOLD_RIGHT_PADDING +
        GetEkeEkeWidth() + EKEEKE_RIGHT_PADDING +
        GetHealthDigitsWidth() + HEALTH_DIGITS_RIGHT_PADDING +
        GetTotalHeartWidth() + HEARTS_RIGHT_PADDING +
        SWORD_CHARGE_LEFT_PADDING + MIN_SWORD_CHARGE_LENGTH + SWORD_CHARGE_RIGHT_PADDING); // Ensure HUD is wide enough to fit all elements

    // Call base class to setup basic tilemap
    UIBox::PrepareTilemap();

    // Additional HUD-specific tile preparations can be done here
    auto col = RenderGold(GetInternalWidthTiles() - GOLD_RIGHT_PADDING);
    col = RenderEkeEke(col - EKEEKE_RIGHT_PADDING);
    col = RenderHealthDigits(col - HEALTH_DIGITS_RIGHT_PADDING);
    col = RenderHealthHearts(col - HEARTS_RIGHT_PADDING);
    col = RenderSwordCharge(SWORD_CHARGE_LEFT_PADDING, col - SWORD_CHARGE_RIGHT_PADDING - SWORD_CHARGE_LEFT_PADDING);
}

unsigned int Hud::GetGoldWidth() const
{
    return std::to_string(GOLD_CAP).length() + 1; // Num digits + 1 tile for symbol
}

unsigned int Hud::GetEkeEkeWidth() const
{
    return std::to_string(EKEEKE_CAP).length() + 2; // Num digits + 2 tiles for symbol
}

unsigned int Hud::GetHealthDigitsWidth() const
{
    const unsigned int health_digits = std::to_string(HEALTH_CAP).length();
    return health_digits * 2 + 1; // current health digits + slash + max health digits
}

unsigned int Hud::GetHealthSmallHeartsWidth() const
{
    return (GetHealthSmallHeartsCount(HEARTS_CAP) + 1) / 2;
}

unsigned int Hud::GetHealthMediumHeartsWidth() const
{
    const unsigned int num_medium_hearts = GetHealthMediumHeartsCount(HEARTS_CAP);
    return num_medium_hearts == 0 ? 0U : num_medium_hearts + 1U;
}

unsigned int Hud::GetHealthLargeHeartsWidth() const
{
    return GetHealthLargeHeartsCount(HEARTS_CAP) * 2U;
}

unsigned int Hud::GetTotalHeartWidth() const
{
    return GetHealthSmallHeartsWidth() + GetHealthMediumHeartsWidth() + GetHealthLargeHeartsWidth();
}

unsigned int Hud::RenderSwordCharge(unsigned int col, unsigned int length)
{
    assert(length >= MIN_SWORD_CHARGE_LENGTH); // Minimum length to render sword charge
    Tile(col, 1) = static_cast<unsigned int>(Tiles::SWORD_HILT_LEFT);
    Tile(col + 1, 0) = static_cast<unsigned int>(Tiles::SWORD_HILT_TOP);
    Tile(col + 1, 1) = static_cast<unsigned int>(Tiles::SWORD_HILT_MIDDLE);
    Tile(col + 1, 2) = static_cast<unsigned int>(Tiles::SWORD_HILT_BOTTOM);

    const int body_segments = length - 3;
    const int max_pixels = body_segments * 8 + 2; // 8 pixels per segment + 2 for tip
    const int charge_pixels = std::clamp(static_cast<int>(this->magic_sword_charge * max_pixels + 0.5), 0, max_pixels);
    const int full_segments = (charge_pixels - 1) / 8;
    const int partial_segment = (charge_pixels - 1) % 8;

    for(unsigned int i = 0; i < body_segments; ++i)
    {
        unsigned int tile_index = static_cast<unsigned int>(Tiles::SWORD_CHARGE_BODY_0);
        if(charge_pixels > 0)
        {
            if(i < full_segments)
            {
                tile_index += 9;
            }
            else if(i == full_segments)
            {
                tile_index += partial_segment + 1;
            }
        }
        Tile(col + i + 2, 1) = tile_index;
    }
    Tile(col + length - 1, 1) = static_cast<unsigned int>(Tiles::SWORD_CHARGE_TIP_0);
    if(full_segments >= body_segments)
    {
        Tile(col + length - 1, 1) = (partial_segment == 0) ? static_cast<unsigned int>(Tiles::SWORD_CHARGE_TIP_1) :
                                                     static_cast<unsigned int>(Tiles::SWORD_CHARGE_TIP_2);
    }
    return col + length;
}

unsigned int Hud::RenderHealthDigits(unsigned int col)
{
    const unsigned int health_digits = std::to_string(HEALTH_CAP).length();
    const unsigned int start_col = col - health_digits * 2 - 1; // current health digits + slash + max health digits
    std::string health_str = std::format("{0:{2}}/{1:^{2}}", std::min(current_health, HEALTH_CAP), std::min(max_health, HEALTH_CAP), health_digits);
    unsigned int cur_col = start_col;
    for (char c : health_str)
    {
        unsigned int tile_top = static_cast<unsigned int>(Tiles::C0_TOP);
        unsigned int tile_bottom = static_cast<unsigned int>(Tiles::C0_BOTTOM);
        if (c >= '0' && c <= '9')
        {
            tile_top += static_cast<unsigned int>(c - '0');
            tile_bottom += static_cast<unsigned int>(c - '0');
            Tile(cur_col, 1) = tile_top;
            Tile(cur_col, 2) = tile_bottom;
        }
        else if (c == '/')
        {
            Tile(cur_col, 1) = static_cast<unsigned int>(Tiles::SLASH_TOP);
            Tile(cur_col, 2) = static_cast<unsigned int>(Tiles::SLASH_BOTTOM);
        }
        ++cur_col;
    }
    return start_col;
}

unsigned int Hud::RenderHealthHearts(unsigned int col)
{
    col = RenderHealthSmallHearts(col);
    col = RenderHealthMediumHearts(col);
    col = RenderHealthLargeHearts(col);
    return col;
}

unsigned int Hud::RenderHealthSmallHearts(unsigned int col)
{
    const unsigned int num_small_hearts = GetHealthSmallHeartsCount(max_health);
    const unsigned int filled_small_hearts = GetHealthSmallHeartsFilled(current_health);
    const unsigned int small_hearts_width = GetHealthSmallHeartsWidth();

    for(unsigned int i = 0; i < small_hearts_width; ++i)
    {
        unsigned int cur_col = col - i - 1;
        bool top_heart_filled = i < filled_small_hearts;
        bool bottom_heart_filled = (i + small_hearts_width) < filled_small_hearts;
        if(i >= num_small_hearts)
        {
            break;
        }
        else if(i + small_hearts_width >= num_small_hearts) // Single heart
        {
            Tile(cur_col, 1) = {static_cast<int>(top_heart_filled ? Tiles::SMALL_HEART_SINGLE_TOP_ROW_FULL : Tiles::SMALL_HEART_SINGLE_TOP_ROW_EMPTY), false, false};
        }
        else
        {
            Tile(cur_col, 1) = {static_cast<int>(top_heart_filled ? Tiles::SMALL_HEART_DOUBLE_TOP_ROW_FULL : Tiles::SMALL_HEART_DOUBLE_TOP_ROW_EMPTY), false, false};
            Tile(cur_col, 2) = {static_cast<int>(bottom_heart_filled ? Tiles::SMALL_HEART_BOTTOM_ROW_FULL : Tiles::SMALL_HEART_BOTTOM_ROW_EMPTY), false, false};
        }
    }

    return col - small_hearts_width;
}

unsigned int Hud::RenderHealthMediumHearts(unsigned int col)
{
    const unsigned int num_medium_hearts = GetHealthMediumHeartsCount(max_health);
    const unsigned int filled_medium_hearts = GetHealthMediumHeartsFilled(current_health);
    const unsigned int medium_hearts_width = GetHealthMediumHeartsWidth();

    if(num_medium_hearts > 0)
    {
        unsigned int cur_col = col - num_medium_hearts - 1;
        bool heart_filled = num_medium_hearts == filled_medium_hearts;
        Texture::Tile tl = {static_cast<int>(heart_filled ? Tiles::MEDIUM_HEART_TOP_LEFT_FULL : Tiles::MEDIUM_HEART_TOP_LEFT_EMPTY), false, false};
        Texture::Tile tr = {static_cast<int>(heart_filled ? Tiles::MEDIUM_HEART_BOTTOM_LEFT_FULL : Tiles::MEDIUM_HEART_BOTTOM_LEFT_EMPTY), false, false};
        Tile(cur_col, 1) = tl;
        Tile(cur_col, 2) = tr;
        ++cur_col;
        for(unsigned int i = num_medium_hearts; i > 1; --i)
        {
            if(filled_medium_hearts == 1 && num_medium_hearts == 2)
            {
                cur_col = cur_col;
            }
            bool right_heart_filled = filled_medium_hearts >= i - 1;
            if(heart_filled)
            {
                Tile(cur_col, 1) = {static_cast<int>(Tiles::MEDIUM_HEART_TOP_OVERLAPPING_BOTH_FULL), false, false};
                Tile(cur_col, 2) = {static_cast<int>(Tiles::MEDIUM_HEART_BOTTOM_OVERLAPPING_BOTH_FULL), false, false};
            }
            else if (right_heart_filled)
            {
                Tile(cur_col, 1) = {static_cast<int>(Tiles::MEDIUM_HEART_TOP_OVERLAPPING_ONE_FULL), false, false};
                Tile(cur_col, 2) = {static_cast<int>(Tiles::MEDIUM_HEART_BOTTOM_OVERLAPPING_ONE_FULL), false, false};
            }
            else
            {
                Tile(cur_col, 1) = {static_cast<int>(Tiles::MEDIUM_HEART_TOP_OVERLAPPING_BOTH_EMPTY), false, false};
                Tile(cur_col, 2) = {static_cast<int>(Tiles::MEDIUM_HEART_BOTTOM_OVERLAPPING_BOTH_EMPTY), false, false};
            }
            heart_filled = right_heart_filled;
            ++cur_col;
        }
        tl = {static_cast<int>(heart_filled ? Tiles::LARGE_HEART_FULL_TOP_RIGHT : Tiles::LARGE_HEART_EMPTY_TOP_RIGHT), false, false};
        tr = {static_cast<int>(heart_filled ? Tiles::LARGE_HEART_FULL_BOTTOM_RIGHT : Tiles::LARGE_HEART_EMPTY_BOTTOM_RIGHT), false, false};
        Tile(cur_col, 1) = tl;
        Tile(cur_col, 2) = tr;
        ++cur_col;
    }

    return col - medium_hearts_width;
}

unsigned int Hud::RenderHealthLargeHearts(unsigned int col)
{
    const unsigned int num_large_hearts = GetHealthLargeHeartsCount(max_health);
    const unsigned int filled_large_hearts = GetHealthLargeHeartsFilled(current_health);    
    const unsigned int large_hearts_width = GetHealthLargeHeartsWidth();

    unsigned int cur_col = col - 2;
    for(unsigned int i = 0; i < num_large_hearts; ++i)
    {
        bool heart_filled = i < filled_large_hearts;
        Tile(cur_col, 1) = static_cast<int>(heart_filled ? Tiles::LARGE_HEART_FULL_TOP_LEFT : Tiles::LARGE_HEART_EMPTY_TOP_LEFT);
        Tile(cur_col + 1, 1) = static_cast<int>(heart_filled ? Tiles::LARGE_HEART_FULL_TOP_RIGHT : Tiles::LARGE_HEART_EMPTY_TOP_RIGHT);
        Tile(cur_col, 2) = static_cast<int>(heart_filled ? Tiles::LARGE_HEART_FULL_BOTTOM_LEFT : Tiles::LARGE_HEART_EMPTY_BOTTOM_LEFT);
        Tile(cur_col + 1, 2) = static_cast<int>(heart_filled ? Tiles::LARGE_HEART_FULL_BOTTOM_RIGHT : Tiles::LARGE_HEART_EMPTY_BOTTOM_RIGHT);
        cur_col -= 2;
    }
    return col - large_hearts_width;
}

unsigned int Hud::RenderEkeEke(unsigned int col)
{
    const unsigned int start_col = col - std::to_string(EKEEKE_CAP).length() - 2; // symbol + dash + digits

    Tile(start_col, 1) = static_cast<unsigned int>(Tiles::EKEEKE_SYM_TOP);
    Tile(start_col, 2) = static_cast<unsigned int>(Tiles::EKEEKE_SYM_BOTTOM);
    Tile(start_col + 1, 1) = static_cast<unsigned int>(Tiles::DASH);

    std::string ekeeke_str = std::to_string(std::min(current_ekeeke, EKEEKE_CAP));
    unsigned int cur_col = start_col + 2;
    for (char c : ekeeke_str)
    {
        unsigned int tile_top = static_cast<unsigned int>(Tiles::C0_TOP);
        unsigned int tile_bottom = static_cast<unsigned int>(Tiles::C0_BOTTOM);
        if (c >= '0' && c <= '9')
        {
            tile_top += static_cast<unsigned int>(c - '0');
            tile_bottom += static_cast<unsigned int>(c - '0');
            Tile(cur_col, 1) = tile_top;
            Tile(cur_col, 2) = tile_bottom;
        }
        ++cur_col;
    }
    return start_col;
}

unsigned int Hud::RenderGold(unsigned int col)
{
    const unsigned int start_col = col - std::to_string(GOLD_CAP).length();

    Tile(col, 1)= static_cast<unsigned int>(Tiles::GOLD_SYM_TOP);
    Tile(col, 2) = static_cast<unsigned int>(Tiles::GOLD_SYM_BOTTOM);

    std::string gold_str = std::to_string(std::min(gold, GOLD_CAP));
    unsigned int cur_col = col - gold_str.length();
    for (char c : gold_str)
    {
        unsigned int tile_top = static_cast<unsigned int>(Tiles::C0_TOP);
        unsigned int tile_bottom = static_cast<unsigned int>(Tiles::C0_BOTTOM);
        if (c >= '0' && c <= '9')
        {
            tile_top += static_cast<unsigned int>(c - '0');
            tile_bottom += static_cast<unsigned int>(c - '0');
            Tile(cur_col, 1) = tile_top;
            Tile(cur_col, 2) = tile_bottom;
        }
        ++cur_col;
    }
    return start_col;
}
