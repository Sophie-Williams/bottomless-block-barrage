#include "window.hpp"
#include "font.hpp"

extern std::unique_ptr<Font> font;

Window::Window(int wx, int wy, int ww, int wh, int style) : Widget(wx, wy, ww, wh, style)
{
}

void Window::create(int wx, int wy, int ww, int wh, int style)
{
    Widget::create(wx, wy, ww, wh, style);
}

void Window::draw()
{
    if (is_hidden()) return;
    sf2d_draw_rectangle(x, y, width, height, RGBA8(0, 0, 255, 255));
}

void Window::draw_text(int wx, int wy, const std::string& str)
{
    font->draw(x + wx, y + wy, str);
}
