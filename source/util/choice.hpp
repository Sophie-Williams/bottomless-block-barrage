#ifndef CHOICE_HPP
#define CHOICE_HPP

#include "widget.hpp"

class Choice : public Widget
{
public:
    Choice() {}
    Choice(int x, int y, int width, int height, const std::vector<std::string>& choice);
    void create(int x, int y, int width, int height, const std::vector<std::string>& choice);
    void update();
    void draw();
    int selection() const {return index;}
    void set_selection(int i) {index = i;}
    void set_choices(const std::vector<std::string>& c);
private:
    std::vector<std::string> choices;
    int index = 0;
};


#endif

