#include "texture.hpp"

Texture::Texture(int width, int height, sf2d_texfmt pixel_format, sf2d_place place) :
    texture(sf2d_create_texture(width, height, pixel_format, place))
{
}

Texture::Texture(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format, sf2d_place place) :
    texture(sf2d_create_texture_mem_RGBA8(src_buffer, sw, sh, pixel_format, place))
{

}

Texture::~Texture()
{
    if (texture)
        sf2d_free_texture(texture);
    texture = nullptr;
}

void Texture::create(int width, int height, sf2d_texfmt pixel_format, sf2d_place place)
{
    if (valid()) return;

    texture = sf2d_create_texture(width, height, pixel_format, place);

}

void Texture::create(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format, sf2d_place place)
{
    if (valid()) return;

    texture = sf2d_create_texture_mem_RGBA8(src_buffer, sw, sh, pixel_format, place);
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


void Texture::draw(int x, int y, int tx, int ty, int tw, int th, u32 blend_color)
{
    if (!valid()) return;
    sf2d_draw_texture_part_blend(texture, x, y, tx, ty, tw, th, blend_color);
}
