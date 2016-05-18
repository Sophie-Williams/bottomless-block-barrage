#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <3ds.h>

class Cursor
{
public:
    Cursor(int w, int h, u32 min, u32 max, int speed, u32 b = 0xFFFFFFFF) :
        width(w), height(h), color(min), border(b), modulate_min(min), modulate_max(max), modulate_speed(speed), direction(true) {}
    void update();
    void draw(int ox = 0, int oy = 0);
private:
    int width;
    int height;
    int color;
    int border;
    const int modulate_min;
    const int modulate_max;
    const int modulate_speed;
    bool direction;
};


#endif
