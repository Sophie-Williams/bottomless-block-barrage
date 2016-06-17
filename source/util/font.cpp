#include "font.hpp"
#include <sstream>

void Font::draw(const std::string& str, int x, int y, u32 color) const
{
    impl.drawStr(str, x, y, color);
}

void Font::draw(int val, int x, int y, u32 color) const
{
    std::stringstream str;
    str << val;
    impl.drawStr(str.str(), x, y, color);
}

void Font::draw(const std::string& str, int x, int y, int w, int h, u32 color, Alignment align) const
{
    u32 dimw, dimh;
    impl.getTextDims(str, dimw, dimh, w);

    int effx, effy;

    if (align & LEFT)
        effx = x;
    else if (align & CENTER_H)
        effx = x + (w - dimw) / 2;
    else if (align & RIGHT)
        effx = x + (w - dimw);
    else
        /// TODO spit out a warning and below.
        effx = x;

    if (align & TOP)
        effy = y;
    else if (align & CENTER_V)
        effy = y + (h - dimh) / 2;
    else if (align & BOTTOM)
        effy = y + (h - dimh);
    else
        effy = y;

    impl.drawStrWrap(str, effx, effy, w, color);
}
