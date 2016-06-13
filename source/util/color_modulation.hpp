#ifndef COLOR_MODULATION_HPP
#define COLOR_MODULATION_HPP

#include <3ds.h>
#include <sf2d.h>

class ColorModulation
{
public:
    ColorModulation() {}
    ColorModulation(u32 min, u32 max, int speed);
    void set(u32 min, u32 max, int speed);
    u32 color() const {return RGBA8(r, g, b, a);}
    u32 start() const {return RGBA8(minr, ming, minb, mina);}
    u32 end() const {return RGBA8(maxr, maxg, maxb, maxa);}
    void update();
private:
    u8 r, g, b, a;
    u8 minr, ming, minb, mina;
    u8 maxr, maxg, maxb, maxa;
    int speed;
    int count = 0;
};

#endif
