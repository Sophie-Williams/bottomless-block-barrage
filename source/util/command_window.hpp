#ifndef COMMAND_WINDOW_HPP
#define COMMAND_WINDOW_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "color_modulation.hpp"
#include "hid_helper.hpp"
#include "window.hpp"


class Cursor
{
public:
    Cursor(int w, int h, u32 min = 0x609F9060, u32 max = 0xA0FFB080, int speed = 32, u32 b = 0xFFFFD0C0) :
        width(w), height(h), border(b), color(min, max, speed) {}
    void update();
    void draw(int ox = 0, int oy = 0, bool disabled = false);
private:
    int width;
    int height;
    int border;
    ColorModulation color;
};

class CommandWindow : public Window
{
public:
    CommandWindow(int wx, int wy, int command_width, int command_height, int items_per_row, const std::vector<std::string>& commands);
    virtual ~CommandWindow() {}
    void update();
    void draw();

    int rows() const {return std::max(choices.size() / items_per_row, 1U);}
    int columns() const {return items_per_row;}
    int selection() const {return index;}

    void set_choices(const std::vector<std::string>& new_choices) {choices = new_choices;}
private:
    std::vector<std::string> choices;
    int command_width;
    int command_height;
    int items_per_row;
    Cursor cursor;
    int index;
    KeyRepeatStore repeat;
};

#endif
