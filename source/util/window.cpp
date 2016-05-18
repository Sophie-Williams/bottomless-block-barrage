#include "window.hpp"

#include <sf2d.h>

Window::Window(int wx, int wy, int ww, int wh) : x(wx), y(wy), width(ww), height(wh), active(false), hidden(false)
{
}

void Window::draw()
{
    sf2d_draw_rectangle(x, y, width, height, RGBA8(0, 0, 255, 255));
}

void Window::center(int sw, int sh)
{
    x = (sw - width) / 2;
    y = (sh - height) / 2;
}
