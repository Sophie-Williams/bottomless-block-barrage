#include "replay_scene.hpp"
#include "title_scene.hpp"
#include "game_common.hpp"
#include <algorithm>
#include <ctime>
#include <map>

#include "panels_gfx.h"
#include "border_gfx.h"
#include "selector_gfx.h"
#include "debug_text.h"

ReplayScene::ReplayScene(const Config& c) : config(c)
{

}

void ReplayScene::initialize()
{
    recorder.load(config.replay_filename);
    level = recorder.level;
    srand(recorder.seed);

    switch (recorder.difficulty)
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

    info.set_level(level);
    info.set_difficulty(static_cast<Difficulty>(recorder.difficulty));

    panels.create(panels_gfx, PANELS_GFX_WIDTH, PANELS_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    selector.create(selector_gfx, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    border.create(border_gfx, BORDER_GFX_WIDTH, BORDER_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    debug.create(debug_text, DEBUG_TEXT_WIDTH, DEBUG_TEXT_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM);

    frames.reset();
}

void ReplayScene::update()
{
    u32 held = 0;
    u32 trigger = 0;
    recorder.keys(trigger, held);

    info.update();

    if (last_frame == 0)
        last_frame = osGetTime();

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), 250, 1, 75, held))
        selector_x = std::max(std::min(selector_x - 1, 4), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), 250, 1, 75, held))
        selector_x = std::max(std::min(selector_x + 1, 4), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), 250, 1, 75, held))
        selector_y = std::max(std::min(selector_y - 1, 10), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), 250, 1, 75, held))
        selector_y = std::max(std::min(selector_y + 1, 10), 0);
    if (trigger & KEY_A)
        panel_table.swap(selector_y, selector_x);
    if (hidKeysDown() & KEY_START)
        current_scene = new TitleScene();

    if (panel_table.is_rised())
    {
        selector_y = std::max(std::min(selector_y - 1, 10), 0);
        last_rise = osGetTime();
    }

    if (last_rise == 0)
        last_rise = osGetTime();

    int max_wait = get_current_speed(level);

    if (held & KEY_R)
    {
        info.clear_timeout();
    }

    MatchInfo minfo = panel_table.update(osGetTime() - last_frame, max_wait, held & KEY_R);
    if (minfo.empty() && !last_match.empty())
    {
        if (last_match.is_timeout())
        {
            int timeout = calculate_timeout(last_match.combo, last_match.cascade + 1,
                                            3 - (int)recorder.difficulty, panel_table.is_danger());
            panel_table.set_timeout(timeout);
            info.set_timeout(timeout);
        }
        info.start_timeout_timer();
        last_match = minfo;
    }

    ccc_stats.set_matchinfo(minfo);
    if (minfo.matched())
    {
        info.pause_timeout_timer();
        score += calculate_score(minfo.combo, minfo.cascade);
        info.set_score(score);

        experience += 1;
        int needed = get_exp_to_level(level);
        while (needed != -1 && experience >= needed)
        {
            level += 1;
            experience -= needed;
            needed = get_exp_to_level(level);
        }
        info.set_level(level);
        info.set_experience(experience, needed);
        if (minfo.is_timeout())
        {
            int timeout = calculate_timeout(minfo.combo, minfo.cascade + 1,
                                            3 - (int)recorder.difficulty, panel_table.is_danger());
            info.set_timeout(timeout);
        }
        last_match = minfo;
    }

    frames.update(panel_table.get_state(), panel_table.is_warning());
    last_frame = osGetTime();
    recorder.update();
    frame++;
}

void ReplayScene::draw_top()
{
    info.draw();
    ccc_stats.draw();
}

void ReplayScene::draw_bottom()
{
    draw_panels();
    draw_selector();
    draw_board();
}

void ReplayScene::draw_panels()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    const int panel_size = PANEL_SIZE + 2;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    if (!panel_table.is_puzzle() && (panel_table.is_clogged() || panel_table.is_gameover() || panel_table.is_rised()))
        offset = panel_size;

    for (int i = 0; i < panel_table.height(); i++)
    {
        for (int j = 0; j < panel_table.width(); j++)
        {
            const Panel& panel = panel_table.get(i, j);
            if (panel.value == Panel::EMPTY) continue;

            int x = startx + j * panel_size + panel_size / 2 + 2;
            int y = starty + (i + 1) * panel_size + panel_size / 2 - offset + 2;

            if (panel.is_right_swap()) x -= panel_size / 2;
            if (panel.is_left_swap()) x += panel_size / 2;

            panels.draw(x, y, (panel.value - 1) * PANEL_SIZE, panel.frame(frames.panel) * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
        }
    }

    for (int j = 0; j < panel_table.width(); j++)
    {
        const int i = panel_table.height();
        const Panel& panel = panel_table.get(i, j);
        int status = (panel_table.is_gameover() ? 7 : 4);

        int x = j * panel_size + 2 + startx + panel_size / 2;
        int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;

        panels.draw(x, y, (panel.value - 1) * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
    }
}

void ReplayScene::draw_selector()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    const int panel_size = PANEL_SIZE + 2;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    if (!panel_table.is_puzzle() && (panel_table.is_clogged() || panel_table.is_gameover() || panel_table.is_rised()))
        offset = panel_size;

    int x = startx + selector_x * panel_size + panel_size / 2 - 2;
    int y = starty + (selector_y + 1) * panel_size + panel_size / 2 - offset - 2;
    selector.draw(x, y, 0, frames.selector * SELECTOR_GFX_HEIGHT / 2, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT / 2);
}

void ReplayScene::draw_board()
{
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height();
    border.draw(startx, starty);
}

