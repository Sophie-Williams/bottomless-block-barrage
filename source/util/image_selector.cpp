#include "image_selector.hpp"

ImageSelector::ImageSelector(int x, int y, int width, int height) : Widget(x, y, width, height), index(0)
{

}

void ImageSelector::create(int x, int y, int width, int height)
{
    Widget::create(x, y, width, height);
    index = 0;
}

void ImageSelector::update()
{
    if (!is_active()) return;

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
}

void ImageSelector::add(const void* src_buffer, int pitch)
{
    images.emplace_back(src_buffer, pitch == -1 ? width : pitch, height);
}
