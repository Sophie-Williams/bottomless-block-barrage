#include "puzzle_scene.hpp"
#include "puzzle_select_scene.hpp"

#include "endless_scene.hpp"

#include "basic_puzzle.hpp"
#include "puzzle_panel_source.hpp"
#include "mode_select_scene.hpp"
#include "game_common.hpp"
#include "puzzle_set.hpp"

#include <cassert>

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

    if (puzzle.type != PUZZLE)
        return false;

    if (puzzle.rows != MAX_PUZZLE_ROWS || puzzle.columns != MAX_PUZZLE_COLUMNS || puzzle.starting != MAX_PUZZLE_ROWS ||
        puzzle.moves > MAX_PUZZLE_MOVES)
        return false;

    opts.type = PanelTable::Type::MOVES;
    opts.rows = puzzle.rows;
    opts.columns = puzzle.columns;
    opts.moves = puzzle.moves;
    opts.source = new PuzzlePanelSource(puzzle);
    return true;
}

PuzzleSnapshot::PuzzleSnapshot(const PanelTable& table)
{
    const auto& table_panels = table.get_panels();
    panels.resize(table_panels.size());
    for (unsigned int i = 0; i < panels.size(); i++)
        panels[i] = (Panel::Type) table_panels[i].get_value();
    moves = table.get_moves();
}

void PuzzleSnapshot::restore(PanelTable& table) const
{
    auto& table_panels = table.get_panels();
    for (unsigned int i = 0; i < panels.size(); i++)
        table_panels[i].set_value(panels[i]);
    table.set_moves(moves);
}

PuzzleScene::PuzzleScene(const GameConfig& config) : GameScene(config),
    status_window(config.level_name)
{

}

void PuzzleScene::init_panel_table()
{
    PanelTable::Options opts;
    assert(read_puzzle(config.puzzle_filename, opts));
    table.reset(new PanelTable(opts));
    snapshots.push_back(PuzzleSnapshot(*table));
}

void PuzzleScene::init_menu()
{
    GameScene::init_menu();
    status_window.set_moves(table->get_moves());
}

void PuzzleScene::update_input()
{
    GameScene::update_input();
    if (input->trigger(KEY_Y))
    {
        const PuzzleSnapshot& snapshot = snapshots.back();
        snapshot.restore(*table);
        if (snapshots.size() != 1)
            snapshots.pop_back();
        status_window.set_moves(table->get_moves());
    }
}

void PuzzleScene::update_on_swap()
{
    if (table->all_idle())
        snapshots.push_back(*table);
    table->swap(selector_y, selector_x);
    status_window.set_moves(table->get_moves());
}

void PuzzleScene::update_windows()
{
    time_window.update();
}

void PuzzleScene::update_on_gameover()
{
    if (table->is_gameover())
        result_text.create("Game Over\nPress Any Button", 0, 0);
    else if (table->is_win())
        result_text.create("Level clear\nPress Any Button", 0, 0);

    result_text.center(TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT);
}

void PuzzleScene::update_gameover()
{
    if (input->trigger(KEY_A))
    {
        if (table->is_gameover())
        {
            current_scene = new PuzzleScene(config);
        }
        else if (table->is_win())
        {
            GameConfig next;
            std::string set_name = config.set_name;
            unsigned int level = config.level_id + 1;
            unsigned int stage = config.stage_id;

            const std::vector<std::string>& stages = PuzzleSelectScene::puzzles[set_name].stage_names;
            const std::string stage_name = stages[stage];
            const std::vector<std::string>& levels = PuzzleSelectScene::puzzles[set_name].stages[stage_name].levels;

            if (level >= levels.size())
            {
                stage += 1;
                level = 0;
                if (stage > stages.size())
                {
                    current_scene = new PuzzleSelectScene();
                    return;
                }
            }

            next.set_name = set_name;
            next.level_id = level;
            next.stage_id = stage;
            next.level_name = PuzzleSelectScene::puzzles[set_name].stages[stages[stage]].levels[level];
            next.puzzle_filename = construct_puzzle_filename(set_name, stages[stage], next.level_name);
            current_scene = new PuzzleScene(next);
        }
    }
}

bool PuzzleScene::is_gameover() const
{
    return table->is_gameover() || table->is_win();
}

void PuzzleScene::draw_bottom()
{
    GameScene::draw_bottom();
    time_window.draw();
    status_window.draw();
}

void PuzzleScene::draw_gameover_top()
{
    result_text.draw();
}

