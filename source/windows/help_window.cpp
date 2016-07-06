#include "help_window.hpp"

HelpWindow::HelpWindow(int x, int y, int width, int height, u64 speed) : Window(x, y, width, height), speed_ms(speed)
{
    set_contents_opacity(0.5f);
    set_border_opacity(0.5f);
}

void HelpWindow::create(int x, int y, int width, int height, u64 speed)
{
    Window::create(x, y, width, height);
    speed_ms = speed;
    set_contents_opacity(0.5f);
    set_border_opacity(0.5f);
}

void HelpWindow::set_text(const std::string& str)
{
    text = str;
    pos = 0;
}

void HelpWindow::update()
{
    if (!is_active()) return;

    if (last_frame_ms == 0)
        last_frame_ms = osGetTime();

    if (osGetTime() - last_frame_ms > speed_ms)
    {
        pos += 1;
        last_frame_ms = osGetTime();
    }
}

void HelpWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();
    if (pos < text.size())
    {
        std::string str = text.substr(0, pos);
        draw_text(str, 0, 0, width, height, 0xFFFFFFFF, LEFT_TOP);
    }
    else
        draw_text(text, 0, 0, width, height, 0xFFFFFFFF, LEFT_TOP);
}
