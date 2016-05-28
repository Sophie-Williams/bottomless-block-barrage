#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <3ds.h>
#include "color_modulation.hpp"

class Cursor
{
public:
    Cursor(int w, int h, u32 min = 0x609F9060, u32 max = 0xA0FFB080, int speed = 32, u32 b = 0xFFFFD0C0) :
        width(w), height(h), border(b), color(min, max, speed) {}
    void update();
    void draw(int ox = 0, int oy = 0, bool disabled = false);
private:
    int width;
    int height;
    int border;
    ColorModulation color;
};


#endif
