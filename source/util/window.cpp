#include "window.hpp"
#include "font.hpp"

extern std::unique_ptr<Font> font;

WindowSkin Window::skin;

void WindowSkin::create(const void* src_buffer, sf2d_texfmt pixel_format, sf2d_place place)
{
    skin.create(src_buffer, WINDOW_SKIN_WIDTH, WINDOW_SKIN_HEIGHT, pixel_format, place);
}

void WindowSkin::draw(int x, int y, int w, int h)
{
    skin.draw(x, y, WINDOW_BG_X, WINDOW_BG_Y, WINDOW_BG_WIDTH, WINDOW_BG_HEIGHT, w, h);

    skin.draw(x, y, WINDOW_BORDER_TL_X, WINDOW_BORDER_TL_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);
    skin.draw(x + w - WINDOW_BORDER_SIZE, y, WINDOW_BORDER_TR_X, WINDOW_BORDER_TR_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);
    skin.draw(x, y + h - WINDOW_BORDER_SIZE, WINDOW_BORDER_BL_X, WINDOW_BORDER_BL_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);
    skin.draw(x + w - WINDOW_BORDER_SIZE, y + h - WINDOW_BORDER_SIZE, WINDOW_BORDER_BR_X, WINDOW_BORDER_BR_Y, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);

    skin.draw(x + WINDOW_BORDER_SIZE, y, WINDOW_BORDER_TL_X + WINDOW_BORDER_SIZE, WINDOW_BORDER_TL_Y, WINDOW_BORDER_WIDTH - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, w - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);
    skin.draw(x, y + WINDOW_BORDER_SIZE, WINDOW_BORDER_TL_X, WINDOW_BORDER_TL_Y + WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, WINDOW_BORDER_HEIGHT - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, h - 2 * WINDOW_BORDER_SIZE);
    skin.draw(x + WINDOW_BORDER_SIZE, y + h - WINDOW_BORDER_SIZE, WINDOW_BORDER_BL_X + WINDOW_BORDER_SIZE, WINDOW_BORDER_BL_Y, WINDOW_BORDER_WIDTH - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, w - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);
    skin.draw(x + w - WINDOW_BORDER_SIZE, y + WINDOW_BORDER_SIZE, WINDOW_BORDER_TR_X, WINDOW_BORDER_TR_Y + WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, WINDOW_BORDER_HEIGHT - 2 * WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE, h - 2 * WINDOW_BORDER_SIZE);
}

Window::Window(int wx, int wy, int ww, int wh, int style)
{
    create(wx, wy, ww, wh, style);
}

void Window::create(int wx, int wy, int ww, int wh, int style)
{
    Widget::create(wx, wy, ww + WINDOW_BORDER_SIZE * 2, wh + WINDOW_BORDER_SIZE * 2, style);
}

void Window::draw()
{
    if (is_hidden()) return;
    if (!skin.valid())
        sf2d_draw_rectangle(x, y, width, height, RGBA8(0, 0, 255, 255));
    else
        skin.draw(x, y, width, height);
}

void Window::draw_text(int wx, int wy, const std::string& str)
{
    font->draw(x + wx + WINDOW_BORDER_SIZE, y + wy + WINDOW_BORDER_SIZE, str);
}

void Window::set_skin(const void *src_buffer, sf2d_texfmt pixel_format, sf2d_place place)
{
    skin.create(src_buffer, pixel_format, place);
}
