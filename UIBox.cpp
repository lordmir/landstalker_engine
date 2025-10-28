#include "UIBox.h"

UIBox::UIBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x, unsigned int y, unsigned int width, unsigned int height, float x_scale, float y_scale)
    : ui_tiles(std::make_unique<Texture>(*ui_tiles))
    , palette(std::make_unique<Palette>(*palette))
    , x(x), y(y)
    , width(width), height(height), x_scale(x_scale), y_scale(y_scale)
{
    this->ui_tiles->SetColours(palette->GetSdlColours());
}

void UIBox::SetBackgroundColour(const SDL_Color& colour)
{
    palette->SetSdlColour(box_background_idx, colour);
    ui_tiles->SetColours(palette->GetSdlColours());
}

void UIBox::Draw(SDL_Renderer *renderer)
{
    for (unsigned int row = 0; row < height + 2; ++row)
    {
        for (unsigned int col = 0; col < width + 2; ++col)
        {
            Texture::Tile tile = {};
            // Determine which tile to use based on position
            if (row == 0 && col == 0)
            {
                tile.index = static_cast<unsigned int>(Tiles::TOP_CORNER);
            }
            else if (row == 0 && col == width + 1)
            {
                tile.index = static_cast<unsigned int>(Tiles::TOP_CORNER);
                tile.hflip = true;
            }
            else if (row == height + 1 && col == 0)
            {
                tile.index = static_cast<unsigned int>(Tiles::BOTTOM_CORNER);
            }
            else if (row == height + 1 && col == width + 1)
            {
                tile.index = static_cast<unsigned int>(Tiles::BOTTOM_CORNER);
                tile.hflip = true;
            }
            else if (row == 0)
            {
                tile.index = static_cast<unsigned int>(Tiles::TOP_EDGE);
            }
            else if (row == height + 1)
            {
                tile.index = static_cast<unsigned int>(Tiles::BOTTOM_EDGE);
            }
            else if (col == 0)
            {
                tile.index = static_cast<unsigned int>(Tiles::VERTICAL_EDGE);
            }
            else if (col == width + 1)
            {
                tile.index = static_cast<unsigned int>(Tiles::VERTICAL_EDGE);
                tile.hflip = true;
            }
            else
            {
                tile.index = static_cast<unsigned int>(Tiles::BODY);
            }
            ui_tiles->DrawTile(renderer, tile, static_cast<float>(x + col * ui_tiles->GetTileWidth() * x_scale), static_cast<float>(y + row * ui_tiles->GetTileHeight() * y_scale), x_scale, y_scale);
        }
    }
}
