#include "slider.hpp"
#include <algorithm>
#include <3ds.h>
#include <sf2d.h>
#include <cstdio>
#include <scenes/scene.hpp>

Slider::Slider(int mi, int ma, int val, int wx, int wy, int ww, int wh) :
    min(mi), max(ma), value(val), x(wx), y(wy), width(ww), height(wh), hidden(false), active(false),
    color(0xFF787878, 0xFFE0E0E0, 16)
{

}

void Slider::update()
{
    if (!is_active()) return;

    color.update();

    u32 trigger = hidKeysDown();

    if (trigger & KEY_LEFT)
        value -= 1;
    if (trigger & KEY_RIGHT)
        value += 1;

    value = std::max(std::min(value, max), min);
}

void Slider::draw()
{
    if (is_hidden()) return;

    sf2d_draw_rectangle(x, y, width, height, RGBA8(0x40, 0x40, 0x40, 255));
    int percent = value * width / (max - min);
    sf2d_draw_rectangle(x + percent - 2, y - 6 + height / 2, 5, 12, RGBA8(0x60, 0x60, 0x60, 255));
    sf2d_draw_rectangle(x + percent - 2 + 1, y - 6 + height / 2 + 1, 3, 10, !is_active() ? color.start() : color.color());
}

void Slider::center(int sw, int sh)
{
    x = (sw - width) / 2;
    y = (sh - height) / 2;
}
