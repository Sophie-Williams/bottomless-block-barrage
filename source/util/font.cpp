#include "font.hpp"
#include <sstream>

void Font::draw(const std::string& str, int x, int y, u32 color)
{
    impl.drawStr(str, x, y, color);
}

void Font::draw(int val, int x, int y, u32 color)
{
    std::stringstream str;
    str << val;
    impl.drawStr(str.str(), x, y, color);
}

void Font::draw(const std::string& str, int x, int y, int w, int h, u32 color, Alignment align)
{

}

