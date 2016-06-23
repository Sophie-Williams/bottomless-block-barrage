#include "texture.hpp"

Texture::Texture(int width, int height, sf2d_texfmt pixel_format, sf2d_place place) :
    texture(sf2d_create_texture(width, height, pixel_format, place))
{
}

Texture::Texture(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format, sf2d_place place) :
    texture(sf2d_create_texture_mem_RGBA8(src_buffer, sw, sh, pixel_format, place))
{

}

void Texture::create(int width, int height, sf2d_texfmt pixel_format, sf2d_place place)
{
    if (valid()) return;
    texture.reset(sf2d_create_texture(width, height, pixel_format, place));
}

void Texture::create(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format, sf2d_place place)
{
    if (valid()) return;
    texture.reset(sf2d_create_texture_mem_RGBA8(src_buffer, sw, sh, pixel_format, place));
}

void Texture::draw(int x, int y)
{
    if (!valid()) return;
    sf2d_draw_texture(texture.get(), x, y);
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th)
{
    if (!valid()) return;
    sf2d_draw_texture_part(texture.get(), x, y, tx, ty, tw, th);
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th, int sw, int sh)
{
    if (!valid()) return;
    sf2d_draw_texture_part_scale(texture.get(), x, y, tx, ty, tw, th, 1.0f * sw / tw, 1.0f * sh / th);
}

void Texture::draw(int x, int y, int tx, int ty, int tw, int th, u32 blend_color)
{
    if (!valid()) return;
    sf2d_draw_texture_part_blend(texture.get(), x, y, tx, ty, tw, th, blend_color);
}

void Texture::draw(int x, int y, int sx, int sy, int sw, int sh, float opacity)
{
    if (!valid()) return;
    unsigned char percent = 255 * opacity;
    sf2d_draw_texture_part_blend(texture.get(), x, y, sx, sy, sw, sh, RGBA8(0xff, 0xff, 0xff, percent));
}
