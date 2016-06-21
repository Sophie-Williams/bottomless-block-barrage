#include "ccc_window.hpp"
#include <scenes/scene.hpp> // for TOP_SCREEN_WIDTH
#include <cstdio>


CCCWindow::CCCWindow() : Window(TOP_SCREEN_WIDTH - 64 - WINDOW_BORDER_SIZE * 2, 0, 64, 96)
{

}

void CCCWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    char buf[9];

    sprintf(buf, "%d", info.chain);
    draw_text("Chain", 0, 0);
    draw_text(buf, 0, 16, 64, 16, 0xFFFFFFFF, RIGHT_CENTER);

    sprintf(buf, "%d", info.combo);
    draw_text("Combo", 0, 32);
    draw_text(buf, 0, 48, 64, 16, 0xFFFFFFFF, RIGHT_CENTER);

    sprintf(buf, "%d", info.clink);
    draw_text("C-link", 0, 64);
    draw_text(buf, 0, 80, 64, 16, 0xFFFFFFFF, RIGHT_CENTER);
}
