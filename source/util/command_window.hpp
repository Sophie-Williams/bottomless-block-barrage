#ifndef COMMAND_WINDOW_HPP
#define COMMAND_WINDOW_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "cursor.hpp"
#include "window.hpp"

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
};

#endif
