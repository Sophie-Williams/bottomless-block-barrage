#include "time_window.hpp"
#include <cstdio>

TimeWindow::TimeWindow() : Window(0, 32, 64, 32)
{
    start_time = osGetTime();
}

void TimeWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    char buf[9];

    u64 time = (osGetTime() - start_time) / 1000;
    int min = std::min(time / 60, (u64)99);
    int sec = time % 60;
    sprintf(buf, "%d'%02d", min, sec);
    draw_text("Time:", 0, 0);
    draw_text(buf, 0, 16, width, 16, 0xFFFFFFFF, RIGHT_CENTER);
}
