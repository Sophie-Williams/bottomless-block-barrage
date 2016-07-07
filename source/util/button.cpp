#include "button.hpp"

#define MIN_COLOR 0x609F9060U
#define MAX_COLOR 0xA0FFB080U
#define COLOR_SPEED 32

Button::Button(const std::string& path, int x, int y) : Widget(x, y, 0, 0), image(path), blend_color(MIN_COLOR, MAX_COLOR, COLOR_SPEED)
{
    width = image.width();
    height = image.height();
}

void Button::create(const std::string& path, int x, int y)
{
    Widget::create(x, y, 0, 0);
    image.create(path);
    width = image.width();
    height = image.height();
    blend_color.set(MIN_COLOR, MAX_COLOR, COLOR_SPEED);
}

void Button::update()
{
    if (!is_active()) return;

    blend_color.update();
}

void Button::draw()
{
    if (is_hidden()) return;

    image.draw(x, y);

    if (!is_active()) return;

    sf2d_draw_rectangle(x, y, width, height, blend_color.color());
}
