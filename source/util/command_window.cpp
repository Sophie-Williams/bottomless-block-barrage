#include "command_window.hpp"
#include <3ds.h>

CommandWindow::CommandWindow(int wx, int wy, int width, const std::vector<std::string>& commands) :
    Window(wx, wy, width, commands.size() * 16), choices(commands), cursor(width, 16, 0x609F9060, 0xA0FFB080, 32, 0xFFFFD0C0), index(0) {}

void CommandWindow::update()
{
    if (!is_active())
        return;

    u32 trigger = hidKeysDown();

    if (trigger & KEY_UP)
        index = (index - 1 + choices.size()) % choices.size();
    if (trigger & KEY_DOWN)
        index = (index + 1) % choices.size();
    cursor.update();
}

void CommandWindow::draw()
{
    Window::draw();
    for (unsigned int i = 0; i < choices.size(); i++)
        font->draw(choices[i], x, y + i * 16, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
    if (is_active() && !is_hidden())
        cursor.draw(x, y + index * 16);
}
