#include "bar.hpp"


Bar::Bar(int val, int max, int x, int y, int width, int height, u32 fore, u32 back) : Widget(x, y, width, height),
    value(val), maximum(max), foreground(fore), background(back)
{
}

void Bar::create(int val, int max, int x, int y, int width, int height, u32 fore, u32 back)
{
    Widget::create(x, y, width, height);
    value = val;
    maximum = max;
    foreground = fore;
    background = back;
}

void Bar::draw()
{
    sf2d_draw_rectangle(x, y, width, height, background);
    int percent = value * (width - 2) / maximum;
    sf2d_draw_rectangle(x + 1, y + 1, percent, height - 2, foreground);
}
