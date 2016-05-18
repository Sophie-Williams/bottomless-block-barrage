#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include "font.hpp"

extern std::unique_ptr<Font> font;

class Window
{
public:
    Window(int wx, int wy, int ww, int wh);
    virtual ~Window() {}
    virtual void update() {}
    virtual void draw();
    void set_active(bool a) {active = a;}
    bool is_active() const  {return active;}
    void set_hidden(bool h) {hidden = h;}
    bool is_hidden() const {return hidden;}
    void center(int sw, int sh);
    void set_width(int w) {width = w;}
    void set_height(int h) {height = h;}
protected:
    int x;
    int y;
    int width;
    int height;
    bool active;
    bool hidden;
};

#endif
