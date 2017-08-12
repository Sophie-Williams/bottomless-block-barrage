#include "info_window.hpp"
#include <algorithm>
#include <cstdio>
#include <scenes/scene.hpp> // for TOP_SCREEN_HEIGHT

InfoWindow::InfoWindow() : Window(0, 0, 8 * 16, TOP_SCREEN_HEIGHT - WINDOW_BORDER_SIZE * 2),
speed_exp_bar(0, 100, 40, 124, 6 * 16, 6, 0xFF809018, 0xFF101010),
time_left_bar(0, 1, 40, 190, 6 * 16, 6, 0xFF000080, 0xFF101010),
score(0)
{
    start_time = osGetTime();
}

void InfoWindow::update()
{
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
    draw_text("Time:", 0, 0);
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
