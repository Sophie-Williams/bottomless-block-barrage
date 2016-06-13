#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "color_modulation.hpp"
#include "hid_helper.hpp"
#include "widget.hpp"

class Slider : public Widget
{
public:
    enum Style
    {
        // Default don't show labels
        DEFAULT = 0,
        // Show Current Value
        LABELS = 1,
    };
    Slider() {}
    Slider(int mi, int max, int val, int x, int y, int w, int h, int style = DEFAULT);
    ~Slider() {}
    void create(int mi, int max, int val, int x, int y, int w, int h, int style = DEFAULT);
    void update();
    void draw();
    int get_value() const {return value;}
    void set_value(int v) {value = v;}
private:
    int min;
    int max;
    int value;
    ColorModulation color;
    KeyRepeatStore repeat;
};

#endif
