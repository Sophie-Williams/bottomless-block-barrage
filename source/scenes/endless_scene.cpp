#include "endless_scene.hpp"
#include "title_scene.hpp"
#include <algorithm>
#include <ctime>
#include <map>

#include "panels_gfx.h"
#include "border_gfx.h"
#include "selector_gfx.h"
#include "debug_text.h"

PanelSpeedSettings easy_speed_settings   = {4, 6, 4, 36, 24, FALL_ANIMATION_FRAMES};
//PanelSpeedSettings slow_speed_settings   = {8, 12, 8, 72, 48, FALL_ANIMATION_FRAMES};
PanelSpeedSettings normal_speed_settings = {3, 5, 3, 27, 18, FALL_ANIMATION_FRAMES};
PanelSpeedSettings hard_speed_settings   = {2, 4, 2, 18, 12, FALL_ANIMATION_FRAMES};

const int CHAIN_VALUE[24] =
{
    0, 50, 80, 150, 300, 400,
    500, 700, 900, 1100, 1300, 1500,
    1800, 2100, 2400, 2700, 3000, 3400,
    3800, 4200, 4600, 5000, 5500, 6000
};

int recursive_combo_score(int combo, int combonum)
{
    int scored = 0;
    if (combo > 3 && combo <= 5)
        scored = 10 * (combonum - 2);
    else if (combo > 5)
        scored = 10 * (combonum - combo - 1) + recursive_combo_score(combo - 2, combonum);
    return scored;
}

int calculate_score(int combo_num, int chain_num)
{
    return CHAIN_VALUE[chain_num] +  10 * combo_num + recursive_combo_score(combo_num, combo_num);
}

int calculate_timeout(int combo, int chain, int difficulty, bool in_danger)
{
    int combo_time;
    int chain_time;
    if (!in_danger)
    {
        combo_time = combo / 2 + difficulty - 2;
        chain_time = difficulty + chain + 1;
    }
    else
    {
        combo_time = (combo + 10) * difficulty / 5 + 1;
        chain_time = difficulty * (1 + chain) + 1;
    }
    if (combo <= 3)
        combo_time = 0;
    if (chain <= 1)
        chain_time = 0;

    return std::min(20, std::max(combo_time, chain_time)) * 1000;
}

const std::map<int, int> speed_table
{
    {1,   18000},
    {10,  9000},
    {20,  6000},
    {30,  4000},
    {40,  3000},
    {50,  2000},
    {60,  1800},
    {70,  1600},
    {80,  1400},
    {90,  1200},
    {100, 1000},
};

const std::map<int, int> level_table
{
    {0, 4},
    {10, 6},
    {20, 8},
    {30, 10},
    {40, 11},
    {50, 12},
    {60, 13},
    {70, 14},
    {80, 15},
    {90, 16},
    {100, -1}
};

int get_current_speed(int level)
{
    const auto& it = speed_table.find(level);
    if (it != speed_table.end())
        return it->second;

    int min = level - level % 10;
    int max = min + 10;
    if (min == 0) min = 1;

    int minv = speed_table.at(min);
    int maxv = speed_table.at(max);

    int diff = (maxv - minv) / 10;
    return minv + (level % 10) * diff;
}

int get_exp_to_level(int level)
{
    return level_table.at(level - level % 10);
}

EndlessScene::EndlessScene(const Config& c) : config(c), level(c.level)
{

}

void EndlessScene::initialize()
{
    srand(time(NULL));
    PanelTable* table;
    switch (config.difficulty)
    {
        case EASY:
            table = new PanelTable(11, 6, 5, easy_speed_settings);
            break;
        case NORMAL:
            table = new PanelTable(11, 6, 6, normal_speed_settings);
            break;
        case HARD:
            table = new PanelTable(11, 6, 6, hard_speed_settings);
            break;
    }

    info.reset(new InfoWindow(config.level, config.difficulty));
    ccc_stats.reset(new CCCWindow());
    panel_table.reset(table);

    panels.reset(new Texture(panels_gfx, PANELS_GFX_WIDTH, PANELS_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    selector.reset(new Texture(selector_gfx, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    border.reset(new Texture(border_gfx, BORDER_GFX_WIDTH, BORDER_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    debug.reset(new Texture(debug_text, DEBUG_TEXT_WIDTH, DEBUG_TEXT_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));

    frames.Reset();
}

void EndlessScene::update()
{
    u32 trigger = hidKeysDown();
    u32 held = hidKeysHeld();
    info->update();

    if (last_frame == 0)
        last_frame = osGetTime();

    if (trigger & KEY_LEFT)
        selector_x = std::max(std::min(selector_x - 1, 4), 0);
    if (trigger & KEY_RIGHT)
        selector_x = std::max(std::min(selector_x + 1, 4), 0);
    if (trigger & KEY_UP)
        selector_y = std::max(std::min(selector_y - 1, 10), 0);
    if (trigger & KEY_DOWN)
        selector_y = std::max(std::min(selector_y + 1, 10), 0);
    if (trigger & KEY_A)
        panel_table->swap(selector_y, selector_x);
    if (trigger & KEY_START)
        current_scene = new TitleScene();

    if (panel_table->is_rised())
    {
        selector_y = std::max(std::min(selector_y - 1, 10), 0);
        last_rise = osGetTime();
    }

    if (last_rise == 0)
        last_rise = osGetTime();

    int max_wait = get_current_speed(level);

    MatchInfo minfo = panel_table->update(osGetTime() - last_frame, max_wait, held & KEY_R);
    if (minfo.empty() && !last_match.empty())
    {
        int timeout = calculate_timeout(last_match.combo, last_match.cascade + 1,
                                        3 - (int)config.difficulty, panel_table->is_danger());
        panel_table->set_timeout(timeout);
        info->set_timeout(timeout);
        last_match = minfo;
    }

    ccc_stats->set_matchinfo(minfo);
    if (minfo.matched())
    {
        score += calculate_score(minfo.combo, minfo.cascade);
        info->set_score(score);

        experience += 1;
        int needed = get_exp_to_level(level);
        while (needed != -1 && experience >= needed)
        {
            level += 1;
            experience -= needed;
            needed = get_exp_to_level(level);
        }
        info->set_level(level);
        info->set_experience(experience, needed);
        last_match = minfo;
    }

    frames.Update(*panel_table, panel_table->is_warning());
    last_frame = osGetTime();
    frame++;
}

void EndlessScene::draw_top_left()
{
    info->draw();
    ccc_stats->draw();
}

void EndlessScene::draw_top_right()
{
    info->draw();
    ccc_stats->draw();
}

void EndlessScene::draw_bottom()
{
    static const std::vector<int> panel_fell_frames = {3, 2, 1};
    const int panel_size = PANEL_SIZE + 2;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table->rise / step;

    if (!panel_table->is_puzzle() && (panel_table->is_clogged() || panel_table->is_gameover() || panel_table->is_rised()))
        offset = panel_size;
    int startx = (BOTTOM_SCREEN_WIDTH - border->width()) / 2;
    int starty = (BOTTOM_SCREEN_HEIGHT - border->height());

    // Draw panels
    for (int i = 0; i < panel_table->height(); i++)
    {
        for (int j = 0; j < panel_table->width(); j++)
        {
            const Panel& panel = panel_table->get(i, j);
            int x = j * panel_size + 2 + startx + panel_size / 2;
            int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
            if (panel.is_right_swap()) x -= panel_size / 2;
            if (panel.is_left_swap()) x += panel_size / 2;
            int type = (int)panel.value - 1;
            if (type == -1) continue;

            int frame = frames.panel;
            if (panel.is_removed())
                frame = 5;
            else if (panel.is_falling() || panel.is_fall_end())
                frame = 0;
            else if (panel.is_fell_idle())
                frame = panel_fell_frames[(FALL_ANIMATION_FRAMES - panel.countdown) / FALL_ANIMATION_DELAY];

            panels->draw(x, y, type * PANEL_SIZE, frame * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
        }
    }

    // Draw Next
    for (int j = 0; j < panel_table->width(); j++)
    {
        int i = panel_table->height();
        const Panel& panel = panel_table->get(i, j);

        int type = (int)panel.value - 1;
        int x = j * panel_size + 2 + startx + panel_size / 2;
        int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
        int status = (panel_table->is_gameover() ? 7 : 4);
        panels->draw(x, y, type * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
    }

    /*for (int i = 0; i < panel_table->height() + 1; i++)
    {
        for (int j = 0; j < panel_table->width(); j++)
        {
            const Panel& panel = panel_table->get(i, j);
            int x = j * panel_size + 2 + startx + panel_size / 2;
            int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
            debug->draw(x, y, panel.state * 5, 0, 5, 10);
            debug->draw(x + 11, y + 6, panel.cascade * 5, 0, 5, 10);
        }
    }*/

    // Draw Selector
    {
        int x = selector_x * panel_size - 2 + startx + panel_size / 2;
        int y = (selector_y + 1) * panel_size - 2 - offset + starty + panel_size / 2;
        selector->draw(x, y, 0, frames.selector * SELECTOR_GFX_HEIGHT / 2, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT / 2);
    }

    border->draw(startx, starty);
}
