#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <sf2d.h>
#include <memory>

class Texture
{
public:
    Texture() : texture(nullptr) {}
    Texture(int width, int height, sf2d_texfmt pixel_format = TEXFMT_RGBA8, sf2d_place place = SF2D_PLACE_RAM);
    Texture(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format = TEXFMT_RGBA8, sf2d_place place = SF2D_PLACE_RAM);
    ~Texture() {}
    bool valid() const {return texture.get();}
    void create(int width, int height, sf2d_texfmt pixel_format = TEXFMT_RGBA8, sf2d_place place = SF2D_PLACE_RAM);
    void create(const void *src_buffer, int sw, int sh, sf2d_texfmt pixel_format = TEXFMT_RGBA8, sf2d_place place = SF2D_PLACE_RAM);

    void draw(int x = 0, int y = 0);
    void draw(int x, int y, int sx, int sy, int sw, int sh);
    void draw(int x, int y, int tx, int ty, int tw, int th, int sw, int sh);
    void draw(int x, int y, int sx, int sy, int sw, int sh, u32 blend_color);
    unsigned int width() const {return valid() ? texture->width : 0;}
    unsigned int height() const {return valid() ? texture->height : 0;}

private:
    std::shared_ptr<sf2d_texture> texture;
};

#endif
