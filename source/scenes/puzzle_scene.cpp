#include "puzzle_scene.hpp"

#include "game_common.hpp"

PuzzleScene::PuzzleScene(const PuzzleConfig& config) : GameScene(GameScene::Config()), puzzle_config(config)
{

}

void PuzzleScene::init_panel_table()
{
     panel_table.create(puzzle_config.filename, easy_speed_settings);
     debug.create(panel_table.str(), 0, 0);
}

void PuzzleScene::update_input()
{
    GameScene::update_input();
}

void PuzzleScene::update_windows()
{

}

bool PuzzleScene::is_gameover() const
{
    return panel_table.moves == 0 && panel_table.all_idle();
}

void PuzzleScene::draw_top()
{
    debug.draw();
}

