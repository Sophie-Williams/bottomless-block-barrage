#include "puzzle_scene.hpp"
#include "puzzle_select_scene.hpp"

#include "endless_scene.hpp"

#include "mode_select_scene.hpp"
#include "game_common.hpp"
#include "puzzle_set.hpp"

PuzzleScene::PuzzleScene(const GameConfig& config) : GameScene(config),
    status_window(config.level_name)
{

}

void PuzzleScene::init_panel_table()
{
     panel_table.create(config.puzzle_filename, easy_speed_settings);
     snapshots.push_back(panel_table);
}

void PuzzleScene::init_menu()
{
    GameScene::init_menu();
    status_window.set_moves(panel_table.moves);
}

void PuzzleScene::update_input()
{
    u32 trigger = hidKeysDown();
    u32 held = hidKeysHeld();

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x - 1, panel_table.columns - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x + 1, panel_table.columns - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y - 1, panel_table.rows - 1), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y + 1, panel_table.rows - 1), 0);
    if (trigger & KEY_A)
    {
        if (panel_table.can_swap(selector_y, selector_x))
        {
            if (panel_table.all_idle())
                snapshots.push_back(panel_table);
            panel_table.swap(selector_y, selector_x);
            status_window.set_moves(panel_table.moves);
        }
    }
    if (trigger & KEY_Y)
    {
        panel_table = snapshots.back();
        if (snapshots.size() != 1)
            snapshots.pop_back();
        status_window.set_moves(panel_table.moves);
    }

    if (trigger & KEY_START)
        current_scene = new ModeSelectScene();
}

void PuzzleScene::update_windows()
{
    time_window.update();
}

void PuzzleScene::update_on_gameover()
{
    if (panel_table.is_gameover())
        result_text.create("Game Over\nPress Any Button", 0, 0);
    else if (panel_table.is_win_puzzle())
        result_text.create("Level clear\nPress Any Button", 0, 0);

    result_text.center(TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT);
}


void PuzzleScene::update_gameover()
{
    u32 trigger = hidKeysDown();
    if (trigger & KEY_A)
    {
        if (panel_table.is_gameover())
        {
            current_scene = new PuzzleScene(config);
        }
        else if (panel_table.is_win_puzzle())
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
    return panel_table.is_gameover() || panel_table.is_win_puzzle();
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

