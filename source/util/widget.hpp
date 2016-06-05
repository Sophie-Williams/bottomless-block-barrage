#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <3ds.h>
#include <sf2d.h>

class Widget
{
public:
    Widget(int x, int y, int width, int height, int style = 0);
    virtual ~Widget() {}
    virtual void update() = 0;
    virtual void draw() = 0;
    void set_active(bool a) {active = a;}
    bool is_active() const  {return active;}
    void set_hidden(bool h) {hidden = h;}
    bool is_hidden() const {return hidden;}
    void set_style(int s) {style = s;}
    int get_style() const {return style;}

    void set_x(int xpos) {x = xpos;}
    void set_y(int ypos) {y = ypos;}
    void set_width(int w) {width = w;}
    void set_height(int h) {height = h;}

    void center(int sw, int sh);
protected:
    int x;
    int y;
    int width;
    int height;
    int style;
    bool active;
    bool hidden;

};


#endif

