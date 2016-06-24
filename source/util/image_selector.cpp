#include "image_selector.hpp"

#define DEFAULT_MIN_COLOR 0x00606060U
#define DEFAULT_MAX_COLOR 0x20E0E0E0U
#define DEFAULT_STEPS 32

ImageSelector::ImageSelector(int x, int y, int width, int height) : Widget(x, y, width, height), index(0),
color(DEFAULT_MIN_COLOR, DEFAULT_MAX_COLOR, DEFAULT_STEPS)
{

}

void ImageSelector::create(int x, int y, int width, int height)
{
    Widget::create(x, y, width, height);
    index = 0;
    color.set(DEFAULT_MIN_COLOR, DEFAULT_MAX_COLOR, DEFAULT_STEPS);
}

void ImageSelector::update()
{
    if (!is_active()) return;
    color.update();

    if (hidKeyRepeat(repeat.get(KEY_UP), 250))
        index = (index - 1 + images.size()) % images.size();
    if (hidKeyRepeat(repeat.get(KEY_DOWN), 250))
        index = (index + 1) % images.size();
    if (hidKeyRepeat(repeat.get(KEY_RIGHT), 250))
        index = (index + 1) % images.size();
    if (hidKeyRepeat(repeat.get(KEY_LEFT), 250))
        index = (index - 1 + images.size()) % images.size();
}

void ImageSelector::draw()
{
    if (is_hidden()) return;

    images[index].draw(x, y, 0, 0, width, height);

    if (is_active())
        sf2d_draw_rectangle(x, y, width, height, color.color());
}

void ImageSelector::add(const void* src_buffer, int pitch)
{
    images.emplace_back(src_buffer, pitch == -1 ? width : pitch, height);
}
