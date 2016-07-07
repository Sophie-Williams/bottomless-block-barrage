#include "choice.hpp"
#include <3ds.h>

Choice::Choice(int x, int y, int width, int height, const std::vector<std::string>& choice) : Widget(x, y, width, height),
    choices(choice)
{

}

void Choice::create(int x, int y, int width, int height, const std::vector<std::string>& choice)
{
    Widget::create(x, y, width, height);
    choices = choice;
}

void Choice::update()
{
    if (!is_active()) return;
    u32 trigger = hidKeysDown();
    if (trigger & KEY_RIGHT)
        index = (index + 1) % choices.size();
    else if (trigger & KEY_LEFT)
        index = (index - 1 + choices.size()) % choices.size();
}

void Choice::draw()
{
    if (is_hidden()) return;
    font->draw(choices[index], x, y);
}

void Choice::set_choices(const std::vector<std::string>& c)
{
    choices = c;
    if (index > choices.size())
        index = 0;
}
