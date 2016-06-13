#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>
#include "widget.hpp"

class Window : public Widget
{
public:
    Window() {}
    Window(int wx, int wy, int ww, int wh, int style = 0);
    void create(int wx, int wy, int ww, int wh, int style = 0);
    virtual ~Window() {}
    virtual void update() {}
    virtual void draw();
    void draw_text(int wx, int wy, const std::string& str);
};

#endif
