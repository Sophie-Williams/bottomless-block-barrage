#include "game_scene.hpp"
#include <ctime>

#include "title_scene.hpp"
#include "endless_config_scene.hpp"

#include "game_common.hpp"
#include "panel.hpp"

#include "panels_gfx.h"
#include "panels2_gfx.h"
#include "border_gfx.h"
#include "selector_gfx.h"
#include "debug_text.h"

void GameScene::initialize()
{
    init_recorder();
    srand(recorder.seed);
    init_panel_table();
    init_sprites();
    init_menu();
}

void GameScene::init_recorder()
{
    recorder.seed = (unsigned int) time(NULL);
    recorder.difficulty = config.difficulty;
    recorder.level = config.level;
}

void GameScene::init_panel_table()
{
    switch (config.difficulty)
    {
        case EASY:
            panel_table.create(11, 6, 5, easy_speed_settings);
            markers.create(&easy_speed_settings);
            break;
        case NORMAL:
            panel_table.create(11, 6, 6, normal_speed_settings);
            markers.create(&normal_speed_settings);
            break;
        case HARD:
            panel_table.create(11, 6, 6, hard_speed_settings);
            markers.create(&hard_speed_settings);
            break;
    }

}

void GameScene::init_sprites()
{
    /// TODO make this more extensible
    switch (config.panel_gfx)
    {
        case 0:
            panels.create(panels_gfx, PANELS_GFX_WIDTH, PANELS_GFX_HEIGHT);
            break;
        case 1:
            panels.create(panels2_gfx, PANELS2_GFX_WIDTH, PANELS2_GFX_HEIGHT);
            break;
    }

    selector.create(selector_gfx, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT);
    border.create(border_gfx, BORDER_GFX_WIDTH, BORDER_GFX_HEIGHT);
    debug.create(debug_text, DEBUG_TEXT_WIDTH, DEBUG_TEXT_HEIGHT);

    frames.reset();
}

void GameScene::init_menu()
{
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
}

void GameScene::update()
{
    if (last_frame == 0)
        last_frame = osGetTime();

    if (!is_gameover())
    {
        update_input();
        update_match();
        update_windows();

        if (is_gameover())
        {
            game_over.set_hidden(false);
            try_again.set_hidden(true);
            save_replay_command.set_hidden(false);
            save_replay_command.set_active(true);
        }
    }
    else
    {
        update_gameover();
    }

    last_frame = osGetTime();
    frame++;
}

bool GameScene::is_gameover() const
{
    return panel_table.is_gameover();
}

void GameScene::update_input()
{
    u32 trigger = hidKeysDown();
    u32 held = hidKeysHeld();
    recorder.add(frame, trigger, held);

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), 200, 1, 50, held))
        selector_x = std::max(std::min(selector_x - 1, 4), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), 200, 1, 50, held))
        selector_x = std::max(std::min(selector_x + 1, 4), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), 200, 1, 50, held))
        selector_y = std::max(std::min(selector_y - 1, 10), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), 200, 1, 50, held))
        selector_y = std::max(std::min(selector_y + 1, 10), 0);
    if (trigger & KEY_A)
        panel_table.swap(selector_y, selector_x);
    if (trigger & KEY_START)
        current_scene = new TitleScene();
}

void GameScene::update_gameover()
{
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
                GameScene::Config save_config = config;
                save_config.level = level;
                current_scene = new EndlessConfigScene(save_config);
            }
            else
                current_scene = new TitleScene();
        }
    }
}

void GameScene::update_match()
{
    u32 held = hidKeysHeld();
    u64 current_frame = osGetTime();
    int max_wait = get_current_speed(level);

    if (panel_table.is_rised())
        selector_y = std::max(std::min(selector_y - 1, 10), 0);

    current_match = panel_table.update(current_frame - last_frame, max_wait, held & KEY_R);
    if (current_match.empty() && !last_match.empty())
    {
        if (last_match.is_timeout())
            update_on_timeout();

        update_end_match();

        last_match = current_match;
    }

    if (current_match.matched())
    {
        int x = current_match.x * PANEL_SIZE;
        int y = current_match.y * PANEL_SIZE;
        if (current_match.is_combo())
            markers.add(x, y, Marker::COMBO, current_match.combo);
        if (current_match.is_chain() && current_match.chain != last_match.chain)
            markers.add(x, y + (current_match.is_combo() ? -PANEL_SIZE : 0), Marker::CHAIN, current_match.chain);
        if (current_match.is_clink())
            markers.add(x, y + ((current_match.is_combo() || current_match.is_chain()) ? -PANEL_SIZE : 0), Marker::CLINK, current_match.clink);

        update_score();
        update_level();
        update_on_matched();
        last_match = current_match;
    }

    frames.update(panel_table.get_state(), panel_table.is_warning());
    markers.update();
}

void GameScene::update_on_timeout()
{
    current_timeout = calculate_timeout(last_match.combo, last_match.chain + 1,
                                    3 - (int)config.difficulty, panel_table.is_danger());
    panel_table.set_timeout(current_timeout);
}

void GameScene::update_score()
{
    score += calculate_score(current_match.combo, current_match.chain);
}

void GameScene::update_level()
{
    experience++;
    int needed = get_exp_to_level(level);
    while (needed != -1 && experience >= needed)
    {
        level++;
        experience -= needed;
        needed = get_exp_to_level(level);
    }
}

void GameScene::draw_top()
{
    if (is_gameover())
        draw_gameover_top();
    else
        draw_game_top();
}

void GameScene::draw_bottom()
{
    if (is_gameover())
        draw_gameover_bottom();
    else
        draw_game_bottom();
}

void GameScene::draw_gameover_top()
{
    game_over.draw();
    try_again.draw();
    save_replay_command.draw();
    try_again_command.draw();
}

void GameScene::draw_game_bottom()
{
    draw_panels();
    draw_selector();
    draw_board();
}

void GameScene::draw_gameover_bottom()
{
    draw_game_bottom();
}

void GameScene::draw_panels()
{
    // 2 px addition to accomadate selector.
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    /// TODO extensible panel sizes
    const int panel_size = PANEL_SIZE;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    /// TODO clean up this section of code.
    if (!panel_table.is_puzzle() && (panel_table.is_clogged() || panel_table.is_gameover() || panel_table.is_rised()))
        offset = panel_size;

    for (int i = 0; i < panel_table.height(); i++)
    {
        for (int j = 0; j < panel_table.width(); j++)
        {
            const Panel& panel = panel_table.get(i, j);
            int status = panel.frame(frames.panel);
            if (panel.value == Panel::EMPTY || status == -1) continue;
            if (panel_table.is_gameover()) status = Panel::LOST;

            int x = startx + j * panel_size + panel_size / 2 + 2;
            int y = starty + (i + 1) * panel_size + panel_size / 2 - offset + 2;

            if (panel.is_right_swap()) x -= panel_size / 2;
            if (panel.is_left_swap()) x += panel_size / 2;

            panels.draw(x, y, (panel.value - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
        }
    }

    for (int j = 0; j < panel_table.width(); j++)
    {
        const int i = panel_table.height();
        const Panel& panel = panel_table.get(i, j);
        int status = (panel_table.is_gameover() ? Panel::LOST : Panel::PENDING);

        int x = j * panel_size + 2 + startx + panel_size / 2;
        int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;

        panels.draw(x, y, (panel.value - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
    }

    markers.draw(startx + panel_size / 2 + 2, starty + panel_size + panel_size / 2 - offset + 2);
    /*
    for (int i = 0; i < panel_table.height() + 1; i++)
    {
         for (int j = 0; j < panel_table.width(); j++)
         {
             const Panel& panel = panel_table.get(i, j);
             if (panel.empty()) continue;
             int x = j * panel_size + 2 + startx + panel_size / 2;
             int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
             debug.draw(x, y, panel.state * 5, 0, 5, 10);
             debug.draw(x + 11, y + 6, panel.chain * 5, 0, 5, 10);
        }
    }*/
}

void GameScene::draw_selector()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    const int panel_size = PANEL_SIZE;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    if (!panel_table.is_puzzle() && (panel_table.is_clogged() || panel_table.is_gameover() || panel_table.is_rised()))
        offset = panel_size;

    int x = startx + selector_x * panel_size + panel_size / 2;
    int y = starty + (selector_y + 1) * panel_size + panel_size / 2 - offset - 2;
    selector.draw(x, y, 0, frames.selector * SELECTOR_GFX_HEIGHT / 2, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT / 2);
}

void GameScene::draw_board()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    border.draw(startx, starty);
}
