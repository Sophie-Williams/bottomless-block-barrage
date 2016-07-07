#include "score_info_window.hpp"
#include <algorithm>
#include <cstdio>
#include <scenes/scene.hpp> // for TOP_SCREEN_HEIGHT

ScoreInfoWindow::ScoreInfoWindow() : Window(0, 0, 8 * 16, TOP_SCREEN_HEIGHT - WINDOW_BORDER_SIZE * 2),
speed_exp_bar(0, 100, 40, 124, 6 * 16, 6, 0xFF809018, 0xFF101010),
time_left_bar(0, 1, 40, 190, 6 * 16, 6, 0xFF000080, 0xFF101010),
last_update(0), timeout(0), timeout_started(false), score(0)
{
    start_time = osGetTime();
}

void ScoreInfoWindow::set_timeout(int time)
{
    if (time > timeout)
    {
        timeout = time;
        time_left_bar.set(timeout, timeout);
    }
}

void ScoreInfoWindow::clear_timeout()
{
    timeout = 0;
    time_left_bar.set(0, 1);
}

void ScoreInfoWindow::start_timeout_timer()
{
    timeout_started = true;
}

void ScoreInfoWindow::pause_timeout_timer()
{
    timeout_started = false;
}

void ScoreInfoWindow::update()
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

void ScoreInfoWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    char buf[9];

    u64 time = value / 1000;
    int min = std::min(time / 60, (u64)99);
    int sec = time % 60;
    switch (mode)
    {
        case TIME:
            draw_text("Time Left:", 0, 0);
            sprintf(buf, "%d'%02d", min, sec);
            break;
        case LINES:
            draw_text("Lines Left:", 0, 0);
            sprintf(buf, "%d", (int)value);
            break;
        case MOVES:
            draw_text("Moves Left:", 0, 0);
            sprintf(buf, "%d", (int)value);
            break;
    }
    draw_text(buf, 0, 16, width, 16, 0xFFFFFFFF, RIGHT_CENTER);

    sprintf(buf, "%d", std::min(score, 99999999));
    draw_text("Score:", 0, 32);
    draw_text(buf, 0, 48, width, 16, 0xFFFFFFFF, RIGHT_CENTER);

    sprintf(buf, "%d", level);
    draw_text("Speed:", 0, 64);
    draw_text(buf, 0, 80, width, 16, 0xFFFFFFFF, RIGHT_CENTER);

    draw_text("Exp:", 0, 96);
    speed_exp_bar.draw();

    draw_text("Level:", 0, 128);
    switch(difficulty)
    {
    case Difficulty::EASY:
        draw_text("EASY", 0, 144, width, 16, 0xFFFFFFFF, RIGHT_CENTER);
        break;
    case Difficulty::NORMAL:
        draw_text("NORMAL", 0, 144, width, 16, 0xFFFFFFFF, RIGHT_CENTER);
        break;
    case Difficulty::HARD:
        draw_text("HARD", 0, 144, width, 16, 0xFFFFFFFF, RIGHT_CENTER);
        break;
    }

    draw_text("Timeout: ", 0, 160);
    time_left_bar.draw();
}
