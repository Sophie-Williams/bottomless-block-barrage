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
    PanelTable::Options opts;
    opts.rows = config.rows;
    opts.columns = config.columns;
    opts.type = config.type;
    switch (config.difficulty)
    {
        case EASY:
            opts.source = new RandomPanelSource(opts.rows, opts.columns, 5);
            opts.settings = easy_speed_settings;
            break;
        case NORMAL:
            opts.source = new RandomPanelSource(opts.rows, opts.columns, 6);
            opts.settings = normal_speed_settings;
            break;
        case HARD:
            opts.source = new RandomPanelSource(opts.rows, opts.columns, 6);
            opts.settings = hard_speed_settings;
            break;
    }

    table.reset(new PanelTable(opts));
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
    Scene::update();
    update_read_input();

    if (!is_gameover())
    {
        recorder.add(frame, trigger, held);
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

    if (table->warning())
        scene_music = get_track("Demo_Critical.brstm");

    frame++;
}

bool GameScene::is_gameover() const
{
    return table->is_gameover();
}

void GameScene::update_input()
{
    update_move();
    update_quick_rise();
    update_selector();
    update_exit();
}

void GameScene::update_read_input()
{
    trigger = hidKeysDown();
    held = hidKeysHeld();
}

void GameScene::update_move()
{
    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        update_on_move(-1, 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        update_on_move(1, 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        update_on_move(0, -1);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        update_on_move(0, 1);
}

void GameScene::update_on_move(int x, int y)
{
    selector_x = std::max(std::min(selector_x + x, table->width() - 2), 0);
    selector_y = std::max(std::min(selector_y + y, table->height() - 1), 0);
}

void GameScene::update_selector()
{
    if (trigger & KEY_A || trigger & KEY_B)
        update_on_swap();
}

void GameScene::update_quick_rise()
{
    if (held & KEY_L || held & KEY_R)
        update_on_quick_rise();
}

void GameScene::update_on_quick_rise()
{
    table->quick_rise();
}

void GameScene::update_on_swap()
{
    table->swap(selector_y, selector_x);
}

void GameScene::update_exit()
{
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
    if (table->is_rised())
        selector_y = std::max(std::min(selector_y - 1, table->height() - 1), 0);

    current_match = table->update();

    if (current_match.matched())
    {
        update_create_markers();
        update_score();
        update_level();
        update_on_matched();
        update_on_timeout();
    }

    frames.update(table->get_state(), danger_panel);
    markers.update();
}

void GameScene::update_create_markers()
{
    int x = current_match.x * PANEL_SIZE;
    int y = current_match.y * PANEL_SIZE;
    if (current_match.is_combo())
        markers.add(x, y, Marker::COMBO, current_match.combo);
    if (current_match.is_chain())
        markers.add(x, y + (current_match.is_combo() ? -PANEL_SIZE : 0), Marker::CHAIN, current_match.chain);
    // Note: Not possible to have something be a combo, chain, and clink
    if (current_match.is_clink())
        markers.add(x, y + ((current_match.is_combo() || current_match.is_chain()) ? -PANEL_SIZE : 0), Marker::CLINK, current_match.clink);
}

void GameScene::update_on_timeout()
{
    table->freeze(calculate_timeout(current_match.combo, current_match.chain + 1, config.difficulty, table->warning()));
}

void GameScene::update_score()
{
    score += calculate_score(current_match.combo, current_match.chain);
}

void GameScene::update_level()
{
    next -= current_match.combo;
    if (next <= 0)
    {
        level++;
        next += get_panels_for_level(level);
        table->set_speed(get_speed_for_level(level));
        update_on_level();
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
    batch.start();
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 9 + 4;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height() + 9;
    const int panel_size = PANEL_SIZE;
    int offset = table->get_rise();

    /// TODO clean up this section of code.
    if (table->is_puzzle() || table->is_clogged() || is_gameover() || table->is_rised())
        offset = panel_size;

    std::vector<bool> column_danger = table->danger_columns();

    for (int i = 0; i < table->height(); i++)
    {
        for (int j = 0; j < table->width(); j++)
        {
            const Panel& panel = table->get(i, j);
            int status = get_panel_frame(panel, frames.panel, danger_panel, column_danger[j], is_gameover());
            if (panel.get_value() == Panel::EMPTY || status == -1) continue;

            int x = startx + j * panel_size;
            int y = starty + (i + 1) * panel_size - offset;

            if (panel.is_right_swap()) x -= panel_size / 2;
            if (panel.is_left_swap()) x += panel_size / 2;

            batch.draw(panels, x, y, (panel.get_value() - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
        }
    }

    const auto& next = table->get_next();
    for (int j = 0; j < table->width(); j++)
    {
        const int i = table->height();
        const Panel& panel = next[j];
        int status = get_panel_frame(panel, frames.panel, danger_panel, column_danger[j], is_gameover());

        int x = j * panel_size + startx;
        int y = (i + 1) * panel_size - offset + starty;

        batch.draw(panels, x, y, (panel.get_value() - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
    }
    batch.end();

    markers.draw(startx, starty + panel_size - offset);

    /*for (int i = 0; i < table->height() + 1; i++)
    {
         for (int j = 0; j < table->width(); j++)
         {
             const Panel& panel = table->get(i, j);
             if (panel.empty()) continue;
             int x = startx + j * panel_size;
             int y = starty + (i + 1) * panel_size - offset;
             debug.draw(x, y, panel.get_state() * 5, 0, 5, 10);
             debug.draw(x + 11, y + 6, panel.get_chain() * 5, 0, 5, 10);
        }
    }*/
}

void GameScene::draw_selector()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 9;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height() + 9 - 4;
    const int panel_size = PANEL_SIZE;
    int offset = table->get_rise();

    if (table->is_puzzle() || table->is_clogged() || is_gameover() || table->is_rised())
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
