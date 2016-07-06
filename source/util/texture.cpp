#include "texture.hpp"
#include "resource_manager.hpp"

Texture::Texture(const std::string& path)
{
    create(path);
}

Texture::~Texture()
{
    if (owned) sf2d_free_texture(texture);
    texture = nullptr;
}

void Texture::create(const std::string& path)
{
    if (valid()) return;
    texture = ResourceManager().load_image(path);
    owned = false;
}

void Texture::draw(int x, int y)
{
    if (!valid()) return;
    sf2d_draw_texture(texture, x, y);
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th)
{
    if (!valid()) return;
    sf2d_draw_texture_part(texture, x, y, tx, ty, tw, th);
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th, int sw, int sh)
{
    if (!valid()) return;
    sf2d_draw_texture_part_scale(texture, x, y, tx, ty, tw, th, 1.0f * sw / tw, 1.0f * sh / th);
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th, int sw, int sh, float opacity)
{
    if (!valid()) return;
    unsigned char percent = 255 * opacity;
    sf2d_draw_texture_part_scale_blend(texture, x, y, tx, ty, tw, th, 1.0f * sw / tw, 1.0f * sh / th, RGBA8(0xff, 0xff, 0xff, percent));
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th, u32 blend_color)
{
    if (!valid()) return;
    sf2d_draw_texture_part_blend(texture, x, y, tx, ty, tw, th, blend_color);
}

void Texture::draw(int x, int y, int sx, int sy, int sw, int sh, float opacity)
{
    if (!valid()) return;
    unsigned char percent = 255 * opacity;
    sf2d_draw_texture_part_blend(texture, x, y, sx, sy, sw, sh, RGBA8(0xff, 0xff, 0xff, percent));
}
