#ifndef COMMAND_WINDOW_HPP
#define COMMAND_WINDOW_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "color_modulation.hpp"
#include "hid_helper.hpp"
#include "window.hpp"

#define CURSOR_MIN_COLOR 0x609F9060U
#define CURSOR_MAX_COLOR 0xA0FFB080U
#define CURSOR_COLOR_SPEED 32
#define CURSOR_BORDER 0xFFFFD0C0U

class Cursor
{
public:
    Cursor() {}
    Cursor(int w, int h, u32 min = CURSOR_MIN_COLOR, u32 max = CURSOR_MAX_COLOR, int speed = CURSOR_COLOR_SPEED, u32 b = CURSOR_BORDER) :
        width(w), height(h), border(b), color(min, max, speed) {}
    void set(int w, int h, u32 min = CURSOR_MIN_COLOR, u32 max = CURSOR_MAX_COLOR, int speed = CURSOR_COLOR_SPEED, u32 b = CURSOR_BORDER);
    void update();
    void draw(int ox = 0, int oy = 0, bool disabled = false);
private:
    int width;
    int height;
    u32 border;
    ColorModulation color;
};

class CommandWindow : public Window
{
public:
    CommandWindow() {}
    CommandWindow(int wx, int wy, int command_width, int command_height, int items_per_row, const std::vector<std::string>& commands);
    virtual ~CommandWindow() {}
    void create(int wx, int wy, int command_width, int command_height, int items_per_row, const std::vector<std::string>& commands);
    void update();
    void draw();

    int rows() const {return std::max(choices.size() / items_per_row, 1U);}
    int columns() const {return items_per_row;}
    int selection() const {return index;}
    void set_selection(int sel) {index = sel;}
    std::string choice() const {return choices[index];}
    bool empty() const {return choices.empty();}

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
