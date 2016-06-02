#include "command_window.hpp"
#include <3ds.h>
#include <algorithm>

CommandWindow::CommandWindow(int wx, int wy, int cwidth, int cheight, int ipr, const std::vector<std::string>& commands) :
    Window(wx, wy, cwidth * ipr, std::max(commands.size() / ipr, 1U) * cheight), choices(commands), command_width(cwidth),
    command_height(cheight), items_per_row(ipr), cursor(cwidth, cheight), index(0)
{
}

void CommandWindow::update()
{
    if (!is_active())
        return;

    u32 trigger = hidKeysDown();

    if (trigger & KEY_UP)
        index = (index - items_per_row + choices.size()) % choices.size();
    if (trigger & KEY_DOWN)
        index = (index + items_per_row) % choices.size();
    if (trigger & KEY_RIGHT)
        index = (index + 1) % choices.size();
    if (trigger & KEY_LEFT)
        index = (index - 1 + choices.size()) % choices.size();
    cursor.update();
}

void CommandWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();
    for (unsigned int i = 0; i < choices.size(); i++)
    {
        int mx = i % items_per_row;
        int my = i / items_per_row;
        draw_text(mx * command_width, my * command_height, choices[i]);
    }
    int mx = index % items_per_row;
    int my = index / items_per_row;
    cursor.draw(x + mx * command_width, y + my * command_height, !is_active());
}
