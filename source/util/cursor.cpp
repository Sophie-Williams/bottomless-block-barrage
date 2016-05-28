#include "cursor.hpp"
#include <sf2d.h>

void Cursor::update()
{
    color.update();
}

void Cursor::draw(int ox, int oy, bool disabled)
{
    sf2d_draw_line(ox,         oy,          ox + width, oy,          1, border);
    sf2d_draw_line(ox,         oy + height, ox + width, oy + height, 1, border);
    sf2d_draw_line(ox,         oy,          ox,         oy + height, 1, border);
    sf2d_draw_line(ox + width, oy,          ox + width, oy + height, 1, border);

    sf2d_draw_rectangle(ox + 1, oy + 1, width - 2, height - 2, disabled ? color.start() : color.color());
}
