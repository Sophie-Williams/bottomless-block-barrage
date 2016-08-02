#include "game_scene.hpp"
#include <ctime>

#include "mode_select_scene.hpp"
#include "endless_config_scene.hpp"

#include "game_common.hpp"
#include "panel.hpp"
#include "panels_gfx.hpp"

#include <util/font.hpp>

void GameScene::initialize()
{
    init_recorder();
    srand(recorder.seed);
    init_panel_table();
    init_sprites();
    init_menu();
    scene_music = get_track("Demo.brstm");
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
            break;
        case NORMAL:
            panel_table.create(11, 6, 6, normal_speed_settings);
            break;
        case HARD:
            panel_table.create(11, 6, 6, hard_speed_settings);
            break;
    }
}

void GameScene::init_sprites()
{
    PanelGfxDescription pdscr;
    if (panel_sets.find(config.panel_gfx) == panel_sets.end())
        pdscr = default_set;
    else
        pdscr = panel_sets[config.panel_gfx];
    panels.create(pdscr.gfx);

    switch (config.difficulty)
    {
        case EASY:
            markers.create(&easy_speed_settings);
            break;
        case NORMAL:
            markers.create(&normal_speed_settings);
            break;
        case HARD:
            markers.create(&hard_speed_settings);
            break;
    }

    selector.create("romfs:/graphics/game/selector.png");
    border.create("romfs:/graphics/borders/red.png");
    debug.create("romfs:/graphics/game/debug_text.png");

    frames.reset();
}

void GameScene::init_menu()
{
    background_top.create("romfs:/graphics/backgrounds/demo.png",
                          0, 0, Background::Repeating | Background::TopScreen);
    background_bottom.create("romfs:/graphics/backgrounds/demo.png",
                             0, 0, Background::Repeating | Background::BottomScreen);
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
            update_on_gameover();
    }
    else
    {
        update_gameover();
    }

    if (panel_table.is_warning())
        scene_music = get_track("Demo_Critical.brstm");

    last_frame = osGetTime();
    frame++;
    Scene::update();
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

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x - 1, panel_table.columns - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x + 1, panel_table.columns - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y - 1, panel_table.rows - 1), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y + 1, panel_table.rows - 1), 0);
    if (trigger & KEY_A || trigger & KEY_B)
        panel_table.swap(selector_y, selector_x);
    if (trigger & KEY_START)
        current_scene = new ModeSelectScene();
}

void GameScene::update_gameover()
{
    background_bottom.update();
    background_top.update();
}

void GameScene::update_match()
{
    u32 held = hidKeysHeld();
    u64 current_frame = osGetTime();
    int max_wait = get_current_speed(level);

    if (panel_table.is_rised())
        selector_y = std::max(std::min(selector_y - 1, panel_table.rows - 1), 0);

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
        update_create_markers();
        update_score();
        update_level();
        update_on_matched();
        last_match = current_match;
    }

    frames.update(panel_table.get_state(), danger_panel);
    markers.update();
}

void GameScene::update_create_markers()
{
    int x = current_match.x * PANEL_SIZE;
    int y = current_match.y * PANEL_SIZE;
    if (current_match.is_combo())
        markers.add(x, y, Marker::COMBO, current_match.combo);
    if (current_match.is_chain() && current_match.chain != last_match.chain)
        markers.add(x, y + (current_match.is_combo() ? -PANEL_SIZE : 0), Marker::CHAIN, current_match.chain);
    // Not possible to have something be a combo, chain, and clink
    if (current_match.is_clink() && current_match.clink != last_match.clink)
        markers.add(x, y + ((current_match.is_combo() || current_match.is_chain()) ? -PANEL_SIZE : 0), Marker::CLINK, current_match.clink);
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
    background_top.draw();
    if (is_gameover())
        draw_gameover_top();
    else
        draw_game_top();
}

void GameScene::draw_bottom()
{
    background_bottom.draw();
    if (is_gameover())
        draw_gameover_bottom();
    else
        draw_game_bottom();
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
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 9 + 4;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height() + 9;
    const int panel_size = PANEL_SIZE;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    /// TODO clean up this section of code.
    if (panel_table.is_puzzle() || panel_table.is_clogged() || is_gameover() || panel_table.is_rised())
        offset = panel_size;

    std::vector<bool> column_danger = panel_table.is_danger_columns();

    for (int i = 0; i < panel_table.height(); i++)
    {
        for (int j = 0; j < panel_table.width(); j++)
        {
            const Panel& panel = panel_table.get(i, j);
            int status = panel.frame(frames.panel, danger_panel, column_danger[j]);
            if (panel.value == Panel::EMPTY || status == -1) continue;
            if (is_gameover()) status = Panel::LOST;

            int x = startx + j * panel_size;
            int y = starty + (i + 1) * panel_size - offset;

            if (panel.is_right_swap()) x -= panel_size / 2;
            if (panel.is_left_swap()) x += panel_size / 2;

            panels.draw(x, y, (panel.value - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
        }
    }

    for (int j = 0; j < panel_table.width(); j++)
    {
        const int i = panel_table.height();
        const Panel& panel = panel_table.get(i, j);
        int status = (is_gameover() ? Panel::LOST : Panel::PENDING);

        int x = j * panel_size + startx;
        int y = (i + 1) * panel_size - offset + starty;

        panels.draw(x, y, (panel.value - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
    }

    markers.draw(startx, starty + panel_size - offset);
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
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 9;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height() + 9 - 4;
    const int panel_size = PANEL_SIZE;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    if (panel_table.is_puzzle() || panel_table.is_clogged() || is_gameover() || panel_table.is_rised())
        offset = panel_size;

    int x = startx + selector_x * panel_size;
    int y = starty + (selector_y + 1) * panel_size - offset;
    selector.draw(x, y, 0, frames.selector * selector.height() / 2, selector.width(), selector.height() / 2);
}

void GameScene::draw_board()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    border.draw(startx, starty);
}
