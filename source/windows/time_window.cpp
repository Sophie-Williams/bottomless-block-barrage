#include "time_window.hpp"
#include <cstdio>

TimeWindow::TimeWindow() : Window(0, 0, 64, 32)
{
    start_time = osGetTime();
}

void TimeWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    char buf[9];

    u64 time = (osGetTime() - start_time) / 1000;
    int hr = std::min(time / 3600, (u64)99);
    int min = time / 60 % 60;
    int sec = time % 60;
    sprintf(buf, "%02d:%02d:%02d", hr, min, sec);
    draw_text("Time:", 0, 0);
    draw_text(buf, 0, 16, width, 16, 0xFFFFFFFF, RIGHT_CENTER);
}
