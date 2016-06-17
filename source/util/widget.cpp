#include "widget.hpp"

extern Font* default_font;

Widget::Widget(int wx, int wy, int ww, int wh, int s)
{
    create(wx, wy, ww, wh, s);
}

void Widget::create(int wx, int wy, int ww, int wh, int s)
{
    x = wx;
    y = wy;
    width = ww;
    height = wh;
    style = s;
    active = false;
    hidden = false;
    font_owned = false;
    font = default_font;
}

Widget::~Widget()
{
    if (font_owned)
        delete font;
}

void Widget::center(int sw, int sh)
{
    x = (sw - width) / 2;
    y = (sh - height) / 2;
}

void Widget::move(int dx, int dy)
{
    x += dx;
    y += dy;
}
