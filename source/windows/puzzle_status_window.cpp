#include "puzzle_status_window.hpp"
#include <scenes/scene.hpp>
#include <cstdio>

PuzzleStatusWindow::PuzzleStatusWindow(const std::string& s) : Window(BOTTOM_SCREEN_WIDTH - 80 - 2 * WINDOW_BORDER_SIZE, 32, 80, 64), stage(s), moves(0)
{

}

void PuzzleStatusWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    draw_text("Stage:", 0, 0);
    draw_text(stage, 0, 16, width, 16, 0xFFFFFFFF, RIGHT_CENTER);

    char buf[10];
    sprintf(buf, "%d", moves);
    draw_text("Moves:", 0, 32);
    draw_text(buf, 0, 48, width, 16, 0xFFFFFFFF, RIGHT_CENTER);
}
