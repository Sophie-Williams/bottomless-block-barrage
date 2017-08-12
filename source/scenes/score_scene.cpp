#include "score_scene.hpp"
#include "game_common.hpp"

#include "score_config_scene.hpp"
#include "mode_select_scene.hpp"
#include <ctime>
#include <sstream>

void ScoreScene::initialize()
{
    GameScene::initialize();
    last_frame = osGetTime();
}

void ScoreScene::init_panel_table()
{
    GameScene::init_panel_table();
    switch (config.time_mode)
    {
        case TIME:
            time = config.value * 1000;
            break;
        case MOVES:
            table->set_moves(config.value);
            break;
        default:
            break;
    }
}

void ScoreScene::init_menu()
{
    GameScene::init_menu();

    game_over.create("", 0, 0);
    game_over.set_hidden(true);
    game_over.centerx(TOP_SCREEN_WIDTH);

    try_again_command.create(0, 0, 48, 16, 2, {"Yes", "No"});
    try_again_command.set_hidden(true);
    try_again_command.center(TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT);

    info.set_level(config.level);
    info.set_difficulty(config.difficulty);
    info.set_mode(config.time_mode);
}

bool ScoreScene::is_gameover() const
{
    bool base_game_over = false;
    switch(config.time_mode)
    {
        case TIME:
            base_game_over = time <= 0;
            break;
        case LINES:
            base_game_over = table->get_lines() > config.value;
            break;
        case MOVES:
            base_game_over = table->get_moves() <= 0;
            break;
    }
    return base_game_over || table->is_gameover();
}

void ScoreScene::update()
{
    time -= (int)(osGetTime() - last_frame);
    last_frame = osGetTime();
    GameScene::update();
}

void ScoreScene::update_windows()
{
    GameScene::update_windows();
    if (config.time_mode == TIME)
        info.set_value(time);
    else if (config.time_mode == LINES)
        info.set_value(config.value - table->get_lines());
    else if (config.time_mode == MOVES)
        info.set_value(table->get_moves());

    info.update();
}

void ScoreScene::update_on_gameover()
{
    GameScene::update_on_gameover();

    std::stringstream results;
    results << "Game Over\nFinal Score " << score << "\nPlay Again?";
    game_over.create(results.str(), 0, 0);
    game_over.centerx(TOP_SCREEN_WIDTH);

    game_over.set_hidden(false);
    try_again_command.set_hidden(false);
    try_again_command.set_active(true);
}

void ScoreScene::update_gameover()
{
    GameScene::update_gameover();

    u32 trigger = hidKeysDown();

    try_again_command.update();

    if (trigger & KEY_A)
    {
        if (try_again_command.selection() == 0)
        {
            GameScene::GameConfig save_config = config;
            current_scene = new ScoreConfigScene(save_config);
        }
        else
            current_scene = new ModeSelectScene();
    }
}

void ScoreScene::draw_game_top()
{
    GameScene::draw_game_top();
    info.draw();
}

void ScoreScene::draw_gameover_top()
{
    game_over.draw();
    try_again_command.draw();
}
