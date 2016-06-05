#include "info_window.hpp"
#include <algorithm>
#include <cstdio>
#include <scenes/scene.hpp> // for TOP_SCREEN_HEIGHT

InfoWindow::InfoWindow(int l, Difficulty d) : Window(0, 0, 8 * 16, TOP_SCREEN_HEIGHT),
speed_exp_bar(0, 100, 24, 96, 6 * 16, 6, 0xFF809018, 0xFF101010),
time_left_bar(0, 1, 24, 152, 6 * 16, 6, 0xFF000080, 0xFF101010),
last_update(0), timeout(1), timeout_started(false), score(0), level(l), difficulty(d)
{
    start_time = osGetTime();
}

void InfoWindow::set_timeout(int time)
{
    if (time > timeout)
    {
        timeout = time;
        time_left_bar.set(timeout, timeout);
    }
}

void InfoWindow::clear_timeout()
{
    timeout = 0;
    time_left_bar.set(0, 1);
}

void InfoWindow::start_timeout_timer()
{
    timeout_started = true;
}

void InfoWindow::pause_timeout_timer()
{
    timeout_started = false;
}

void InfoWindow::update()
{
    if (timeout_started)
    {
        timeout -= osGetTime() - last_update;
        if (timeout <= 0)
        {
            timeout = 0;
            time_left_bar.set(0, 1);
            timeout_started = false;
        }
        else
        {
            time_left_bar.set_value(timeout);
        }
    }
    last_update = osGetTime();
}

void InfoWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    char buf[9];

    u64 time = (osGetTime() - start_time) / 1000;
    int hr = std::min(time / 3600, (u64)99);
    int min = time / 60 % 60;
    int sec = time % 60;
    sprintf(buf, "%02d:%02d:%02d", hr, min, sec);
    draw_text(0, 0, "Time:");
    draw_text(0, 16, buf);

    sprintf(buf, "%8d", std::min(score, 99999999));
    draw_text(0, 32, "Score:");
    draw_text(0, 48, buf);

    sprintf(buf, "%8d", level);
    draw_text(0, 64, "Speed:");
    draw_text(0, 80, buf);

    speed_exp_bar.draw();

    draw_text(0, 112, "Level:");
    switch(difficulty)
    {
    case Difficulty::EASY:
        draw_text(64, 128, "EASY");
        break;
    case Difficulty::NORMAL:
        draw_text(32, 128, "NORMAL");
        break;
    case Difficulty::HARD:
        draw_text(64, 128, "HARD");
        break;
    }

    if (timeout) time_left_bar.draw();
}
