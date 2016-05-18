#ifndef COMMAND_WINDOW_HPP
#define COMMAND_WINDOW_HPP

#include <string>
#include <vector>
#include "cursor.hpp"
#include "window.hpp"

class CommandWindow : public Window
{
public:
    CommandWindow(int wx, int wy, int width, const std::vector<std::string>& commands);
    virtual ~CommandWindow() {}
    void update();
    void draw();

    int selection() const {return index;}

    void set_choices(const std::vector<std::string>& new_choices) {choices = new_choices;}
private:
    std::vector<std::string> choices;
    Cursor cursor;
    int index;
};

#endif
