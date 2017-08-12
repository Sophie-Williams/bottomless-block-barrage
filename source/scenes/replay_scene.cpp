#include "replay_scene.hpp"
#include "title_scene.hpp"
#include "game_common.hpp"
#include "panels_gfx.hpp"

void ReplayScene::init_recorder()
{
    recorder.load(config.replay_filename);
    config.level = level = recorder.level;
    config.difficulty = static_cast<Difficulty>(recorder.difficulty);
}

void ReplayScene::init_menu()
{
    GameScene::init_menu();
    info.set_level(config.level);
    info.set_difficulty(config.difficulty);
}

void ReplayScene::update_input()
{
    trigger = 0;
    held = 0;
    recorder.keys(trigger, held);

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x - 1, table->width() - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x + 1, table->width() - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y - 1, table->height() - 1), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y + 1, table->height() - 1), 0);

    if (trigger & KEY_A)
        table->swap(selector_y, selector_x);

    if (hidKeysDown() & KEY_START)
        current_scene = new TitleScene();

    recorder.update();
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
