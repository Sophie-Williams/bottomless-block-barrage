#include "replay_scene.hpp"
#include "title_scene.hpp"
#include "game_common.hpp"
#include "panels_gfx.hpp"

void ReplayScene::initialize()
{
    if (!load_replay(config.replay_filename, replay_info))
        current_scene = new TitleScene();
    GameScene::initialize();
    input.set_data_source(replay_info.input.get());
}

void ReplayScene::init_panel_table()
{
    PanelTable::Options opts;
    opts.rows = replay_info.rows;
    opts.columns = replay_info.columns;
    opts.type = static_cast<PanelTable::Type>(replay_info.type);
    switch (static_cast<Difficulty>(replay_info.difficulty))
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
    }
    opts.source = replay_info.source.get();
    level = replay_info.level;

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
    ccc_stats.draw();
    info.draw();
}

void ReplayScene::draw_game_bottom()
{
    GameScene::draw_game_bottom();
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 9 + 4;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height() + 9;
    const int panel_size = PANEL_SIZE;
    int offset = table->get_rise();

    for (int i = 0; i < table->height() + 1; i++)
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
    }
}
