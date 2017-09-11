#ifndef PRESENT_CONFIGURATION
#define PRESENT_CONFIGURATION

#include <string>
#include "panel_table.hpp"

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
    char panels[72];
};

enum PresetType
{
    PRESET_PUZZLE = 0,
    PRESET_MISSION = 1,
};

bool read_puzzle(const std::string& filename, PanelTable::Options& opts);

#endif
