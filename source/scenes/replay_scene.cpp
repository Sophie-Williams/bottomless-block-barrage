#include "replay_scene.hpp"
#include "title_scene.hpp"
#include "game_common.hpp"
#include "panels_gfx.hpp"

void ReplayScene::initialize()
{
    if (!load_replay(config.replay_filename, replay_info))
        current_scene = new TitleScene();
    input.set_data_source(replay_info.input.get());
    config.columns = replay_info.columns;
    config.rows = replay_info.rows;
    config.level = replay_info.level;
    config.difficulty = static_cast<Difficulty>(replay_info.difficulty);
    config.type = static_cast<PanelTable::Type>(replay_info.type);
    GameScene::initialize();
}

void ReplayScene::init_panel_table()
{
    PanelTable::Options opts;
    opts.rows = config.rows;
    opts.columns = config.columns;
    opts.type = config.type;
    opts.source = replay_info.source.release();
    switch (config.difficulty)
    {
        case EASY:
            opts.settings = easy_speed_settings;
            break;
        case NORMAL:
            opts.settings = normal_speed_settings;
            break;
        case HARD:
            opts.settings = hard_speed_settings;
            break;
        default:
            current_scene = new TitleScene();
            break;
    }
    level = config.level;

    table.reset(new PanelTable(opts));
    table->set_speed(get_speed_for_level(level));
    next = get_panels_for_level(level);
}

void ReplayScene::init_menu()
{
    GameScene::init_menu();
    info.set_level(replay_info.level);
    info.set_difficulty(static_cast<Difficulty>(replay_info.difficulty));
    info.set_panel_table(table.get());
}

void ReplayScene::update_input()
{
    int mx = 0, my = 0;
    if (input.repeat_quick(KEY_LEFT, SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS))
        mx = -1;
    if (input.repeat_quick(KEY_RIGHT, SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS))
        mx = 1;
    if (input.repeat_quick(KEY_UP, SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS))
        my = -1;
    if (input.repeat_quick(KEY_DOWN, SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS))
        my = 1;

    selector_x = std::max(std::min(selector_x + mx, table->width() - 2), 0);
    selector_y = std::max(std::min(selector_y + my, table->height() - 1), 0);

    if (input.held(KEY_L) || input.held(KEY_R))
        table->quick_rise();

    if (input.trigger(KEY_A) || input.trigger(KEY_B))
        table->swap(selector_y, selector_x);

    if (hidKeysDown() & KEY_START)
        current_scene = new TitleScene();

    if (hidKeysDown() & KEY_X)
        debug_drawing = !debug_drawing;
}

void ReplayScene::update_gameover()
{
    GameScene::update_gameover();

    if (hidKeysDown() & KEY_START)
        current_scene = new TitleScene();
}

void ReplayScene::update_windows()
{
    GameScene::update_windows();
    info.update();
    ccc_stats.set_matchinfo(current_match);
}

void ReplayScene::update_on_matched()
{
    GameScene::update_on_matched();
    info.set_score(score);
    info.set_next(get_panels_for_level(level) - next, get_panels_for_level(level));
}

void ReplayScene::update_on_level()
{
    GameScene::update_on_level();
    info.set_level(level);
    info.set_next(get_panels_for_level(level) - next, get_panels_for_level(level));
}

void ReplayScene::draw_game_top()
{
    GameScene::draw_game_top();
    ccc_stats.draw();
    info.draw();
}

void ReplayScene::draw_game_bottom()
{
    GameScene::draw_game_bottom();
}
