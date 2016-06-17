#include "text.hpp"

extern Font* default_font;

Text::Text(const std::string& str, int x, int y, Font* font)
{
    create(str, x, y, font);
}

void Text::create(const std::string& str, int x, int y, Font* f)
{
    u32 width, height;
    if (!f) f = default_font;
    f->dimensions(str, width, height);
    Widget::create(x, y, width, height);
    text = str;
    if (f != default_font)
    {
        font = f;
        font_owned = f != default_font;
    }
}

void Text::draw()
{
    if (is_hidden()) return;
    font->draw(text, x, y);
}
