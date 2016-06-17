#include "command_window.hpp"
#include <3ds.h>
#include <algorithm>

void Cursor::set(int w, int h, u32 min, u32 max, int speed, u32 b)
{
    width = w;
    height = h;
    border = b;
    color.set(min, max, speed);
}

void Cursor::update()
{
    color.update();
}

void Cursor::draw(int ox, int oy, bool disabled)
{
    sf2d_draw_line(ox,         oy,          ox + width, oy,          1, border);
    sf2d_draw_line(ox,         oy + height, ox + width, oy + height, 1, border);
    sf2d_draw_line(ox,         oy,          ox,         oy + height, 1, border);
    sf2d_draw_line(ox + width, oy,          ox + width, oy + height, 1, border);

    sf2d_draw_rectangle(ox + 1, oy + 1, width - 2, height - 2, disabled ? color.start() : color.color());
}

CommandWindow::CommandWindow(int wx, int wy, int cwidth, int cheight, int ipr, const std::vector<std::string>& commands)
{
    create(wx, wy, cwidth, cheight, ipr, commands);
}

void CommandWindow::create(int wx, int wy, int cwidth, int cheight, int ipr, const std::vector<std::string>& commands)
{
    Window::create(wx, wy, cwidth * ipr, std::max(commands.size() / ipr, 1U) * cheight);
    choices = commands;
    command_width = cwidth;
    command_height = cheight;
    items_per_row = ipr;
    cursor.set(cwidth, cheight);
    index = 0;
    alignment = CENTER;
    default_color = 0xFFFFFFFF;
}

void CommandWindow::update()
{
    if (!is_active())
        return;

    if (hidKeyRepeat(repeat.get(KEY_UP), 250))
        index = (index - items_per_row + choices.size()) % choices.size();
    if (hidKeyRepeat(repeat.get(KEY_DOWN), 250))
        index = (index + items_per_row) % choices.size();
    if (hidKeyRepeat(repeat.get(KEY_RIGHT), 250))
        index = (index + 1) % choices.size();
    if (hidKeyRepeat(repeat.get(KEY_LEFT), 250))
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
        u32 color = (colors.find(i) != colors.end()) ? colors[i] : default_color;
        draw_text(choices[i], mx * command_width, my * command_height, command_width, command_height, color, alignment);
    }
    int mx = index % items_per_row;
    int my = index / items_per_row;
    cursor.draw(x + mx * command_width, y + my * command_height, !is_active());
}
