#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <sf2d.h>

class Texture
{
public:
    Texture(int width, int height, sf2d_texfmt pixel_format, sf2d_place place);
    Texture(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format, sf2d_place place);
    ~Texture();
    bool valid() const {return texture;}
    void draw(int x = 0, int y = 0);
    void draw(int x, int y, int sx, int sy, int sw, int sh);
    void draw(int x, int y, int sx, int sy, int sw, int sh, u32 blend_color);
    unsigned int width() const {return valid() ? texture->width : 0;}
    unsigned int height() const {return valid() ? texture->height : 0;}

private:
    sf2d_texture* texture;
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;
};

#endif
