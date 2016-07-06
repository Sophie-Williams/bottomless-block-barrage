#include "window.hpp"
#include "font.hpp"

WindowSkin Window::skin;

void WindowSkin::create(const std::string& path)
{
    skin.create(path);
}

void WindowSkin::draw(int x, int y, int w, int h, float contents_opacity, float border_opacity)
{

    skin.draw(x, y, WINDOW_BG_X, WINDOW_BG_Y, WINDOW_BG_WIDTH, WINDOW_BG_HEIGHT, w, h, contents_opacity);

    skin.draw(x, y, WINDOW_BORDER_TL_X, WINDOW_BORDER_TL_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, border_opacity);
    skin.draw(x + w - WINDOW_BORDER_SIZE, y, WINDOW_BORDER_TR_X, WINDOW_BORDER_TR_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, border_opacity);
    skin.draw(x, y + h - WINDOW_BORDER_SIZE, WINDOW_BORDER_BL_X, WINDOW_BORDER_BL_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, border_opacity);
    skin.draw(x + w - WINDOW_BORDER_SIZE, y + h - WINDOW_BORDER_SIZE, WINDOW_BORDER_BR_X, WINDOW_BORDER_BR_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, border_opacity);

    skin.draw(x + WINDOW_BORDER_SIZE, y, WINDOW_BORDER_TL_X + WINDOW_BORDER_SIZE, WINDOW_BORDER_TL_Y, WINDOW_BORDER_WIDTH - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, w - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, border_opacity);
    skin.draw(x, y + WINDOW_BORDER_SIZE, WINDOW_BORDER_TL_X, WINDOW_BORDER_TL_Y + WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, WINDOW_BORDER_HEIGHT - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, h - 2 * WINDOW_BORDER_SIZE, border_opacity);
    skin.draw(x + WINDOW_BORDER_SIZE, y + h - WINDOW_BORDER_SIZE, WINDOW_BORDER_BL_X + WINDOW_BORDER_SIZE, WINDOW_BORDER_BL_Y, WINDOW_BORDER_WIDTH - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, w - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, border_opacity);
    skin.draw(x + w - WINDOW_BORDER_SIZE, y + WINDOW_BORDER_SIZE, WINDOW_BORDER_TR_X, WINDOW_BORDER_TR_Y + WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, WINDOW_BORDER_HEIGHT - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, h - 2 * WINDOW_BORDER_SIZE, border_opacity);
}

Window::Window(int wx, int wy, int ww, int wh, int style)
{
    create(wx, wy, ww, wh, style);
}

void Window::create(int wx, int wy, int ww, int wh, int style)
{
    Widget::create(wx + WINDOW_BORDER_SIZE, wy + WINDOW_BORDER_SIZE, ww, wh, style);
}

void Window::draw()
{
    if (is_hidden()) return;
    if (!skin.valid())
        sf2d_draw_rectangle(x, y, width, height, RGBA8(0, 0, 255, 255));
    else
        skin.draw(x - WINDOW_BORDER_SIZE, y - WINDOW_BORDER_SIZE, width + WINDOW_BORDER_SIZE * 2, height + WINDOW_BORDER_SIZE * 2, contents_opacity, border_opacity);
}

void Window::draw_text(const std::string& str, int wx, int wy, u32 color)
{
    font->draw(str, x + wx, y + wy, color);
}

void Window::draw_text(const std::string& str, int wx, int wy, int ww, int wh, u32 color, Alignment align)
{
    font->draw(str, x + wx, y + wy, ww, wh, color, align);
}

void Window::set_skin(const std::string& path)
{
    skin.create(path);
}
