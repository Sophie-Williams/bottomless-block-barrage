#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <3ds.h>
#include <sf2d.h>
#include "sfw.hpp"
#include "font.hpp"

class Widget
{
public:
    Widget() {}
    Widget(int x, int y, int width, int height, int style = 0);
    virtual ~Widget();
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void create(int x, int y, int width, int height, int style = 0);
    void set_active(bool a) {active = a;}
    bool is_active() const  {return active;}
    void set_hidden(bool h) {hidden = h;}
    bool is_hidden() const {return hidden;}
    void set_style(int s) {style = s;}
    int get_style() const {return style;}

    int get_x() const {return x;}
    int get_y() const {return y;}
    int get_width() const {return width;}
    int get_height() const {return height;}

    void set_x(int xpos) {x = xpos;}
    void set_y(int ypos) {y = ypos;}
    void set_width(int w) {width = w;}
    void set_height(int h) {height = h;}

    void center(int sw, int sh);
    void centerx(int sw) {x = (sw - width) / 2;}
    void centery(int sh) {y = (sh - height) / 2;}
    void move(int dx, int dy);

    void set_font(Font* newfont, bool own = true) {font = newfont; font_owned = own;}
protected:
    int x;
    int y;
    int width;
    int height;
    int style;
    bool active;
    bool hidden;
    const Font* font;
    bool font_owned;
};


#endif

