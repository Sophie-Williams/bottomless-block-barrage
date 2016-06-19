#ifndef FONT_HPP
#define FONT_HPP

#include <sf2d.h>
#include <string>
#include "BmpFont.h"
#include "sfw.hpp"

class Font
{
public:
    Font(const std::string& filename) : impl(filename.c_str()) {}
    ~Font() {}
    void load(const std::string& filename) {impl.load(filename.c_str());}

    bool valid() const {return impl;}

    void draw(const std::string& str, int x, int y, u32 color = 0xFFFFFFFF) const;
    void draw(int value, int x, int y, u32 color = 0xFFFFFFFF) const;
    void draw(const std::string& str, int x, int y, int w, int h, u32 color = 0xFFFFFFFF, Alignment align = LEFT_TOP) const;

    u32 size() const {return impl.height();}
    u32 width(const std::string& str) const {return impl.getTextWidth(str);}
    u32 height(const std::string& str) const {return impl.getTextHeight(str);}
    void dimensions(const std::string& str, u32& width, u32& height) {return impl.getTextDims(str, width, height);}

private:
    BmpFont impl;
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;
};

#endif
