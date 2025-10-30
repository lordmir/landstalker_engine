#include "UIBox.h"

UIBox::UIBox(std::shared_ptr<Texture> ui_tiles, std::shared_ptr<Palette> palette, unsigned int x, unsigned int y, unsigned int width, unsigned int height, float x_scale, float y_scale)
    : ui_tiles(std::make_unique<Texture>(*ui_tiles))
    , palette(std::make_unique<Palette>(*palette))
    , x(x), y(y), width(0), height(0)
{
    this->ui_tiles->SetColours(palette->GetSdlColours());
    SetXScale(x_scale);
    SetYScale(y_scale);
    SetInternalWidthPixels(width);
    SetInternalHeightPixels(height);
}

unsigned int UIBox::GetX() const
{
    return x;
}

unsigned int UIBox::GetY() const
{
    return y;
}

unsigned int UIBox::GetInternalX() const
{
    return x + tile_width * border_thickness;
}

unsigned int UIBox::GetInternalY() const
{
    return y + tile_height * border_thickness;
}

unsigned int UIBox::GetInternalWidthPixels() const
{
    return width * tile_width;
}

unsigned int UIBox::GetInternalHeightPixels() const
{
    return height * tile_height;
}

unsigned int UIBox::GetInternalWidthTiles() const
{
    return width;
}

unsigned int UIBox::GetInternalHeightTiles() const
{
    return height;
}

unsigned int UIBox::GetExternalWidthPixels() const
{
    return (width + border_thickness * 2) * tile_width;
}

unsigned int UIBox::GetExternalHeightPixels() const
{
    return (height + border_thickness * 2) * tile_height;
}

unsigned int UIBox::GetExternalWidthTiles() const
{
    return width + border_thickness * 2;
}

unsigned int UIBox::GetExternalHeightTiles() const
{
    return height + border_thickness * 2;
}

float UIBox::GetXScale() const
{
    return x_scale;
}

float UIBox::GetYScale() const
{
    return y_scale;
}

SDL_Color UIBox::GetBackgroundColour() const
{
    return palette->GetSdlColour(box_background_idx);
}

void UIBox::SetX(unsigned int nx)
{
    x = nx;
}

void UIBox::SetY(unsigned int ny)
{
    y = ny;
}

void UIBox::SetInternalWidthPixels(unsigned int w)
{
    width = (w + tile_width - 1) / tile_width;
    PrepareTilemap();
}

void UIBox::SetInternalHeightPixels(unsigned int h)
{
    height = (h + tile_height - 1) / tile_height;
    PrepareTilemap();
}

void UIBox::SetInternalWidthTiles(unsigned int w)
{
    width = w; PrepareTilemap();
}

void UIBox::SetInternalHeightTiles(unsigned int h)
{
    height = h; PrepareTilemap();
}

void UIBox::SetXScale(float xs)
{
    x_scale = xs;
    tile_width = static_cast<unsigned int>(ui_tiles->GetTileWidth() * x_scale);
}

void UIBox::SetYScale(float ys)
{
    y_scale = ys;
    tile_height = static_cast<unsigned int>(ui_tiles->GetTileHeight() * y_scale);
}

void UIBox::SetScale(float xs, float ys)
{
    SetXScale(xs); SetYScale(ys);
}

void UIBox::SetScale(float s)
{
    SetXScale(s);
    SetYScale(s);
}

void UIBox::SetBackgroundColour(const SDL_Color& colour)
{
    palette->SetSdlColour(box_background_idx, colour);
    ui_tiles->SetColours(palette->GetSdlColours());
}

void UIBox::SetTile(const Texture::Tile &tile, const unsigned int x, const unsigned int y)
{
    tiles[x + y * GetExternalWidthTiles()] = tile;
}

Texture::Tile UIBox::GetTile(const unsigned int x, const unsigned int y) const
{
    return tiles[x + y * GetExternalWidthTiles()];
}

Texture::Tile& UIBox::Tile(const unsigned int x, const unsigned int y)
{
    return tiles[x + y * GetExternalWidthTiles()];
}

void UIBox::Draw(SDL_Renderer *renderer)
{
    PrepareTilemap();
    for (unsigned int row = 0; row < height + 2; ++row)
    {
        for (unsigned int col = 0; col < width + 2; ++col)
        {
            Texture::Tile tile = tiles.at(row * (width + 2) + col);
            ui_tiles->DrawTile(renderer, tile, static_cast<float>(x + col * ui_tiles->GetTileWidth() * x_scale), static_cast<float>(y + row * ui_tiles->GetTileHeight() * y_scale), x_scale, y_scale);
        }
    }
}

void UIBox::PrepareTilemap()
{
    tiles.resize((height + 2) * (width + 2));
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
            tiles[row * (width + 2) + col]= tile;
        }
    }
}
