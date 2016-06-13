#include "color_modulation.hpp"
#include <algorithm>

ColorModulation::ColorModulation(u32 modulate_min, u32 modulate_max, int spd) : speed(spd)
{
    r = minr = modulate_min >> 0  & 0xFF;
    g = ming = modulate_min >> 8  & 0xFF;
    b = minb = modulate_min >> 16 & 0xFF;
    a = mina = modulate_min >> 24 & 0xFF;

    maxr = modulate_max >> 0  & 0xFF;
    maxg = modulate_max >> 8  & 0xFF;
    maxb = modulate_max >> 16 & 0xFF;
    maxa = modulate_max >> 24 & 0xFF;
}

void ColorModulation::set(u32 min, u32 max, int spd)
{
    r = minr = min >> 0  & 0xFF;
    g = ming = min >> 8  & 0xFF;
    b = minb = min >> 16 & 0xFF;
    a = mina = min >> 24 & 0xFF;

    maxr = max >> 0  & 0xFF;
    maxg = max >> 8  & 0xFF;
    maxb = max >> 16 & 0xFF;
    maxa = max >> 24 & 0xFF;

    speed = spd;
}

void ColorModulation::update()
{
    count = (count + 1) % (2 * speed - 2);
    bool decrease = count / speed;
    int step = count % speed;
    if (decrease) step = speed - 2 - step;

    r = minr + (maxr - minr) * step / speed;
    g = ming + (maxg - ming) * step / speed;
    b = minb + (maxb - minb) * step / speed;
    a = mina + (maxa - mina) * step / speed;
}
