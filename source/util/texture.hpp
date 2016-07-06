#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <sf2d.h>

class Texture
{
public:
    Texture() : texture(nullptr) {}
    Texture(const std::string& path);
    ~Texture();
    bool valid() const {return texture;}
    void create(const std::string& path);

    void draw(int x = 0, int y = 0);
    void draw(int x, int y, int sx, int sy, int sw, int sh);
    void draw(int x, int y, int tx, int ty, int tw, int th, int sw, int sh);
    void draw(int x, int y, int tx, int ty, int tw, int th, int sw, int sh, float opacity);
    void draw(int x, int y, int sx, int sy, int sw, int sh, u32 blend_color);
    void draw(int x, int y, int sx, int sy, int sw, int sh, float opacity);
    unsigned int width() const {return valid() ? texture->width : 0;}
    unsigned int height() const {return valid() ? texture->height : 0;}

private:
    sf2d_texture* texture;
    bool owned = false;
};

#endif
