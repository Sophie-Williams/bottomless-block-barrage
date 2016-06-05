#ifndef FONT_HPP
#define FONT_HPP

#include <sf2d.h>
#include <string>

class Font
{
public:
    Font(const void *src_buffer, int sw, int sh, int cw, int ch, sf2d_texfmt pixel_format, sf2d_place place);
    ~Font();
    bool valid() const {return texture;}
    int char_width() const {return text_width;}
    int char_height() const {return text_height;}
    void draw(int x, int y, const std::string& str);
    void draw(int x, int y, char ch);
    void set_blend_color(u32 color) {blend_color = color;}
    u32 get_blend_color() const {return blend_color;}

private:
    sf2d_texture* texture;
    int text_width;
    int text_height;
    int chars_per_row;
    u32 blend_color;
    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&) = delete;
};

#endif
