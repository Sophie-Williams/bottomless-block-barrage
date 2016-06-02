#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>
#include "widget.hpp"

class Window : public Widget
{
public:
    Window(int wx, int wy, int ww, int wh);
    virtual ~Window() {}
    virtual void update() {}
    virtual void draw();
    void draw_text(int wx, int wy, const std::string& str);
};

#endif
