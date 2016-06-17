///TODO give this mini library a better name.
#ifndef UNNAMED_HPP
#define UNNAMED_HPP

enum Alignment
{
    LEFT = 0,
    CENTER_H = 1,
    RIGHT = 2,
    TOP = 4,
    CENTER_V = 8,
    BOTTOM = 16,

    LEFT_TOP = LEFT|TOP,
    LEFT_CENTER = LEFT|CENTER_V,
    LEFT_BOTTOM = LEFT|BOTTOM,
    CENTER_TOP = CENTER_H|TOP,
    CENTER = CENTER_H|CENTER_V,
    CENTER_BOTTOM = CENTER_H|BOTTOM,
    RIGHT_TOP = RIGHT|TOP,
    RIGHT_CENTER = RIGHT|CENTER_V,
    RIGHT_BOTTOM = RIGHT|BOTTOM,
};

void unnamed_init();
void unnamed_exit();

#endif
