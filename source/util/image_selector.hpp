#ifndef IMAGE_SELECTOR_HPP
#define IMAGE_SELECTOR_HPP

#include "widget.hpp"
#include "texture.hpp"
#include "hid_helper.hpp"

class ImageSelector : public Widget
{
public:
    ImageSelector() {}
    ImageSelector(int x, int y, int width, int height);
    void create(int x, int y, int width, int height);
    void update();
    void draw();
    void add(const void* src_buffer, int pitch = -1);
    int selection() const {return index;}
private:
    std::vector<Texture> images;
    int index;
    KeyRepeatStore repeat;
};


#endif
