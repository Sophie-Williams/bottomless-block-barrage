#include "info_window.hpp"
#include <algorithm>
#include <cstdio>
#include <scenes/scene.hpp> // for TOP_SCREEN_HEIGHT

InfoWindow::InfoWindow(int l, Difficulty d) : Window(0, 0, 8 * 16, TOP_SCREEN_HEIGHT), level(l), difficulty(d)
{
    start_time = osGetTime();
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

    draw_text(0, 96, "Level:");
    switch(difficulty)
    {
    case Difficulty::EASY:
        draw_text(64, 112, "EASY");
        break;
    case Difficulty::NORMAL:
        draw_text(32, 112, "NORMAL");
        break;
    case Difficulty::HARD:
        draw_text(64, 112, "HARD");
        break;
    }
}
