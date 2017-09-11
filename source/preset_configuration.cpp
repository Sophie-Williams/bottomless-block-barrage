#include "preset_configuration.hpp"
#include "puzzle_panel_source.hpp"
#include "game_common.hpp"

#define VERSION_MAJOR 1
#define VERSION_MINOR 1

#define MAX_PUZZLE_ROWS 12
#define MAX_PUZZLE_COLUMNS 6
#define MAX_PUZZLE_MOVES 1000000

bool read_puzzle(const std::string& filename, PanelTable::Options& opts)
{
    BasicPuzzle puzzle;
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file)
        return false;
    fread(&puzzle, sizeof(BasicPuzzle), 1, file);
    fclose(file);

    const char* magic = puzzle.magic;
    if (!(magic[0] == 'B' && magic[1] == 'B' && magic[2] == 'B' && magic[3] == 0))
        return false;

    const char* version = puzzle.version;
    if (version[0] > VERSION_MAJOR || (version[0] == VERSION_MAJOR && version[1] > VERSION_MINOR))
        return false;

    if (puzzle.type != PRESET_PUZZLE)
        return false;

    if (puzzle.rows != MAX_PUZZLE_ROWS || puzzle.columns != MAX_PUZZLE_COLUMNS || puzzle.starting != MAX_PUZZLE_ROWS ||
        puzzle.moves > MAX_PUZZLE_MOVES)
        return false;

    opts.type = PanelTable::Type::MOVES;
    opts.rows = puzzle.rows;
    opts.columns = puzzle.columns;
    opts.moves = puzzle.moves;
    opts.source = new PuzzlePanelSource(puzzle);
    opts.settings = easy_speed_settings;
    return true;
}
