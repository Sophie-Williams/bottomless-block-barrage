#include "endless_scene.hpp"
#include "game_common.hpp"

#include "endless_config_scene.hpp"
#include "mode_select_scene.hpp"
#include <ctime>

void EndlessScene::init_menu()
{
    GameScene::init_menu();

    game_over.create("Game Over\nSave Replay?", 0, 0);
    game_over.set_hidden(true);
    game_over.centerx(TOP_SCREEN_WIDTH);

    try_again.create("Try Again?", 0, 0);
    try_again.set_hidden(true);
    try_again.centerx(TOP_SCREEN_WIDTH);

    save_replay_command.create(0, 0, 48, 16, 2, {"Yes", "No"});
    save_replay_command.set_hidden(true);
    save_replay_command.center(TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT);

    try_again_command.create(0, 0, 48, 16, 2, {"Yes", "No"});
    try_again_command.set_hidden(true);
    try_again_command.center(TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT);

    info.set_level(config.level);
    info.set_difficulty(config.difficulty);
}

void EndlessScene::update_windows()
{
    GameScene::update_windows();
    u32 held = hidKeysHeld();
    if (held & KEY_R)
        info.clear_timeout();
    info.update();
}

void EndlessScene::update_end_match()
{
    GameScene::update_end_match();
    info.start_timeout_timer();
}

void EndlessScene::update_on_timeout()
{
    GameScene::update_on_timeout();
    info.set_timeout(current_timeout);
}

void EndlessScene::update_on_matched()
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

void EndlessScene::update_on_gameover()
{
    GameScene::update_on_gameover();
    game_over.set_hidden(false);
    try_again.set_hidden(true);
    save_replay_command.set_hidden(false);
    save_replay_command.set_active(true);
}

void EndlessScene::update_gameover()
{
    GameScene::update_gameover();

    save_replay_command.update();
    try_again_command.update();
    u32 trigger = hidKeysDown();

    if (save_replay_command.is_active())
    {
        if (trigger & KEY_A)
        {
            if (save_replay_command.selection() == 0)
                recorder.save();

            save_replay_command.set_active(false);
            save_replay_command.set_hidden(true);
            try_again_command.set_active(true);
            try_again_command.set_hidden(false);
            game_over.set_hidden(true);
            try_again.set_hidden(false);
        }
    }
    else if (try_again_command.is_active())
    {
        if (trigger & KEY_A)
        {
            if (try_again_command.selection() == 0)
            {
                GameScene::GameConfig save_config = config;
                save_config.level = level;
                current_scene = new EndlessConfigScene(save_config);
            }
            else
                current_scene = new ModeSelectScene();
        }
    }
}

void EndlessScene::draw_game_top()
{
    GameScene::draw_game_top();
    info.draw();
}

void EndlessScene::draw_gameover_top()
{
    game_over.draw();
    try_again.draw();
    save_replay_command.draw();
    try_again_command.draw();
}
