#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "color_modulation.hpp"
#include "widget.hpp"

class Slider : public Widget
{
public:
    Slider(int mi, int max, int val, int x, int y, int w, int h);
    ~Slider() {}
    void update();
    void draw();
    int get_value() const {return value;}
    void set_value(int v) {value = v;}
private:
    int min;
    int max;
    int value;
    ColorModulation color;
};

#endif
