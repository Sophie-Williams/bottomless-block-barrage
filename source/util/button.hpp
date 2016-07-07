#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "color_modulation.hpp"
#include "texture.hpp"
#include "widget.hpp"

class Button : public Widget
{
public:
    Button() {}
    Button(const std::string& path, int x, int y);
    void create(const std::string& path, int x, int y);
    void update();
    void draw();
private:
    Texture image;
    ColorModulation blend_color;
};

#endif
