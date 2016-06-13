#include "widget.hpp"

Widget::Widget() : x(0), y(0), width(0), height(0), style(0), active(false), hidden(false)
{
}

Widget::Widget(int wx, int wy, int ww, int wh, int s) : x(wx), y(wy), width(ww), height(wh), style(s), active(false),
    hidden(false)
{
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
