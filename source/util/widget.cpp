#include "widget.hpp"

Widget::Widget(int wx, int wy, int ww, int wh) : x(wx), y(wy), width(ww), height(wh), active(false), hidden(false)
{

}

void Widget::center(int sw, int sh)
{
    x = (sw - width) / 2;
    y = (sh - height) / 2;
}
