#include "font.hpp"

Font::Font(const void *src_buffer, int sw, int sh, int cw, int ch, sf2d_texfmt pixel_format, sf2d_place place) :
    texture(sf2d_create_texture_mem_RGBA8(src_buffer, sw, sh, pixel_format, place)), text_width(cw), text_height(ch),
    chars_per_row(sw / cw), blend_color(0xFFFFFFFF)
{
}

Font::~Font()
{
    if (texture)
        sf2d_free_texture(texture);
    texture = NULL;
}

void Font::draw(int x, int y, const std::string& str)
{
    if (!valid()) return;
    for (unsigned int i = 0; i < str.size(); i++)
        draw(x + text_width * i, y, str[i]);
}

void Font::draw(int x, int y, char ch)
{
    if (!valid()) return;
    int tx = ch % chars_per_row;
    int ty = ch / chars_per_row;
    sf2d_draw_texture_part_blend(texture, x, y, tx * text_width, ty * text_height, text_width, text_height, blend_color);
}
