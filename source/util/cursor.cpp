#include "cursor.hpp"
#include <sf2d.h>

void Cursor::update()
{
    char r, g, b, a;
    char rs, gs, bs, as;
    rs = ((modulate_max >> 0  & 0xFF) - (modulate_min >> 0 & 0xFF)) / modulate_speed;
    gs = ((modulate_max >> 8  & 0xFF) - (modulate_min >> 8  & 0xFF)) / modulate_speed;
    bs = ((modulate_max >> 16 & 0xFF) - (modulate_min >> 16  & 0xFF)) / modulate_speed;
    as = ((modulate_max >> 24 & 0xFF) - (modulate_min >> 24 & 0xFF)) / modulate_speed;

    r = (color >> 0  & 0xFF) + (direction ? 1 : -1) * rs;
    g = (color >> 8  & 0xFF) + (direction ? 1 : -1) * gs;
    b = (color >> 16 & 0xFF) + (direction ? 1 : -1) * bs;
    a = (color >> 24 & 0xFF) + (direction ? 1 : -1) * as;

    color = RGBA8(r, g, b, a);

    if (color == modulate_min || color == modulate_max)
        direction = !direction;
}

void Cursor::draw(int ox, int oy)
{
    sf2d_draw_line(ox,         oy,          ox + width, oy,          1, border);
    sf2d_draw_line(ox,         oy + height, ox + width, oy + height, 1, border);
    sf2d_draw_line(ox,         oy,          ox,         oy + height, 1, border);
    sf2d_draw_line(ox + width, oy,          ox + width, oy + height, 1, border);

    sf2d_draw_rectangle(ox + 1, oy + 1, width - 2, height - 2, color);
}
