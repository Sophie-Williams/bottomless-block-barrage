#include "puzzle_scene.hpp"

#include "title_scene.hpp"
#include "game_common.hpp"

PuzzleScene::PuzzleScene(const PuzzleConfig& config) : GameScene(GameScene::Config()), puzzle_config(config)
{

}

void PuzzleScene::init_panel_table()
{
     panel_table.create(puzzle_config.filename, easy_speed_settings);
     snapshots.push_back(panel_table);
}

void PuzzleScene::init_menu()
{
    GameScene::init_menu();
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
        }
    }
    if (trigger & KEY_Y)
    {
        panel_table = snapshots.back();
        if (snapshots.size() != 1)
            snapshots.pop_back();
    }

    if (trigger & KEY_START)
        current_scene = new TitleScene();
}

void PuzzleScene::update_windows()
{
    time_window.update();
}

bool PuzzleScene::is_gameover() const
{
    return panel_table.moves == 0 && panel_table.all_idle();
}

void PuzzleScene::draw_bottom()
{
    GameScene::draw_bottom();
    time_window.draw();
}

