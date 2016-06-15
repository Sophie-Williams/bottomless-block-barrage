#ifndef FONT_HPP
#define FONT_HPP

#include <sf2d.h>
#include <string>
#include "BmpFont.h"

class Font
{
public:
    Font(const std::string& filename) : impl(filename.c_str()) {}
    ~Font() {}
    void load(const std::string& filename) {impl.load(filename.c_str());}
    bool valid() const {return impl;}
    void draw(const std::string& str, int x, int y, u32 color = 0xFFFFFFFF);
    void draw(int value, int x, int y, u32 color = 0xFFFFFFFF);
    void draw(const std::string& str, int x, int y, int w, int h, u32 color = 0xFFFFFFFF);

private:
    BmpFont impl;
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;
};

#endif
