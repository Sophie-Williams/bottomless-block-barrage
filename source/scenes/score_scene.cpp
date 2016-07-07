#include "score_scene.hpp"
#include "game_common.hpp"

#include "score_config_scene.hpp"
#include "mode_select_scene.hpp"
#include <ctime>
#include <sstream>

void ScoreScene::init_panel_table()
{
    GameScene::init_panel_table();
    switch (config.time_mode)
    {
        case TIME:
            time = config.value * 1000;
            break;
        case MOVES:
            panel_table.moves = config.value;
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
            base_game_over = panel_table.lines > config.value;
            break;
        case MOVES:
            base_game_over = panel_table.moves <= 0;
            break;
    }
    return base_game_over || panel_table.is_gameover();
}

void ScoreScene::update_windows()
{
    GameScene::update_windows();
    time -= (int)(osGetTime() - last_frame);
    if (config.time_mode == TIME)
        info.set_value(time);
    else if (config.time_mode == LINES)
        info.set_value(config.value - panel_table.lines);
    else if (config.time_mode == MOVES)
        info.set_value(panel_table.moves);

    u32 held = hidKeysHeld();
    if (held & KEY_R)
        info.clear_timeout();
    info.update();
}

void ScoreScene::update_end_match()
{
    GameScene::update_end_match();
    info.start_timeout_timer();
}

void ScoreScene::update_on_timeout()
{
    GameScene::update_on_timeout();
    info.set_timeout(current_timeout);
}

void ScoreScene::update_on_matched()
{
    GameScene::update_on_matched();
    int needed = get_exp_to_level(level);
    info.set_score(score);
    info.pause_timeout_timer();
    info.set_level(level);
    info.set_experience(experience, needed);
    if (current_match.is_timeout())
    {
        int timeout = calculate_timeout(current_match.combo, current_match.chain + 1,
                                        3 - (int)config.difficulty, panel_table.is_danger());
        info.set_timeout(timeout);
    }
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
