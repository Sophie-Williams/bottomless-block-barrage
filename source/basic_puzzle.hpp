#ifndef BASIC_PUZZLE_HPP
#define BASIC_PUZZLE_HPP

struct BasicPuzzle
{
    char magic[4];
    char version[2];
    char type;
    char pad1;
    int rows;
    int columns;
    int starting;
    int moves;
    char panels[66];
    char pad2[6];
};

extern char* ta_stages[20];

#endif
