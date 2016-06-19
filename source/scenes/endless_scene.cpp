#include "endless_scene.hpp"
#include "game_common.hpp"

#include <algorithm>
#include <ctime>

void EndlessScene::init_menu()
{
    GameScene::init_menu();
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
    ccc_stats.set_matchinfo(current_match);
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
        int timeout = calculate_timeout(current_match.combo, current_match.cascade + 1,
                                        3 - (int)config.difficulty, panel_table.is_danger());
        info.set_timeout(timeout);
    }
}

void EndlessScene::draw_game_top()
{
    ccc_stats.draw();
    info.draw();
}
