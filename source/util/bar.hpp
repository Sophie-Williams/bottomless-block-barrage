#ifndef BAR_HPP
#define BAR_HPP

#include "widget.hpp"

class Bar : public Widget
{
public:
    Bar() {}
    Bar(int val, int max, int x, int y, int width, int height, u32 foreground, u32 background);
    virtual ~Bar() {}
    virtual void create(int val, int max, int x, int y, int width, int height, u32 foreground, u32 background);
    virtual void update();
    virtual void draw();
    void set(int val, int max) {value = val; maximum = max;}
    void set_value(int val) {value = val;}
    void set_max(int max) {maximum = max;}
    void set_foreground(u32 fore) {foreground = fore;}
    void set_background(u32 back) {background = back;}
protected:
    int value;
    int maximum;
    u32 foreground;
    u32 background;
};

#endif

