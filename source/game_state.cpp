#include "game_state.hpp"
#include "panels_gfx.h"
#include "selector_gfx.h"
#include "border_gfx.h"
#include "debug_text.h"

#include <map>

#define PANEL_SIZE 16
#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240

u32 getKeyState()
{
    static u32 last_held = 0;
    u32 held = hidKeysHeld();

    u32 ret = (~last_held) & held;
    last_held = held;
    return ret;
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

int get_current_speed(int level)
{
    const auto& it = speed_table.find(level);
    if (it != speed_table.end())
        return it->second;

    int min = level / 10 * 10;
    int max = min + 10;
    if (min == 0) min = 1;

    int minv = speed_table.at(min);
    int maxv = speed_table.at(max);

    int diff = (maxv - minv) / 10;
    return minv + (level % 10) * diff;
}

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

void AnimationParams::Update(const PanelTable::State& state, bool danger)
{
    static const std::vector<int> panel_frames = {0, 1, 2, 1, 0, 3};
    if (!danger)
        panel_counter = 0;

    if ((state.is_rising() && !danger) || state.is_rised() || state.is_stopped() || state.is_stopping())
        panel = 0;
    else if (state.is_gameover())
        panel = 7;
    else if (state.is_clogged())
        panel = 2;
    else if (state.is_rising() && danger)
    {
        panel_counter++;
        panel = panel_frames[(panel_counter >> 2) % panel_frames.size()];
    }

    selector_counter++;
    selector = ((selector_counter >> 4) & 1);
}

void GameState::Init(const Options& opts)
{
    score = 0;
    level = 1;
    selector_x = 2;
    selector_y = 6;
    panels.reset(new Texture(panels_gfx, PANELS_GFX_WIDTH, PANELS_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    selector.reset(new Texture(selector_gfx, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    border.reset(new Texture(border_gfx, BORDER_GFX_WIDTH, BORDER_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    debug.reset(new Texture(debug_text, DEBUG_TEXT_WIDTH, DEBUG_TEXT_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    panel_table.reset(new PanelTable(opts.rows, opts.columns, opts.colors));
    frames.Reset();
    last_rise = last_frame = 0;
}

void GameState::Update()
{
    u32 trigger = getKeyState();
    u32 held = hidKeysHeld();

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
    if (trigger & KEY_SELECT)
        Init();
    if (trigger & KEY_A)
        panel_table->swap(selector_y, selector_x);
    if (trigger & KEY_X)
        level++;
    if (trigger & KEY_Y)
        level--;

    if (panel_table->get_state().is_rised())
    {
        selector_y = std::max(std::min(selector_y - 1, 10), 0);
        last_rise = osGetTime();
    }

    if (last_rise == 0)
        last_rise = osGetTime();

    int max_wait = get_current_speed(level);

    panel_table->update(osGetTime() - last_frame, max_wait, held & KEY_R);

    frames.Update(panel_table->get_state(), panel_table->is_warning());
    last_frame = osGetTime();
}

void GameState::Render()
{
    const PanelTable::State& state = panel_table->get_state();
    const int panel_size = PANEL_SIZE + 2;
    const int step = get_current_speed(level) / panel_size;
    int offset = state.value() / step;
    if (state.is_clogged() || state.is_gameover() || state.is_rised())
        offset = panel_size;
    int startx = (BOTTOM_SCREEN_WIDTH - border->width()) / 2;
    int starty = (BOTTOM_SCREEN_HEIGHT - border->height());

    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
    for (int i = 0; i < panel_table->height(); i++)
    {
        for (int j = 0; j < panel_table->width(); j++)
        {
            const Panel& panel = panel_table->get(i, j);
            const Panel::State& state = panel.get_state();
            int x = j * panel_size + 2 + startx + panel_size / 2;
            int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
            if (state.is_right_swap()) x -= panel_size / 2;
            if (state.is_left_swap()) x += panel_size / 2;
            int type = (int)panel.value - 1;
            if (type == -1) continue;
            panels->draw(x, y, type * PANEL_SIZE, frames.panel * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
        }
    }

    for (int j = 0; j < panel_table->width(); j++)
    {
        int i = panel_table->height();
        const Panel& panel = panel_table->get(i, j);

        int type = (int)panel.value - 1;
        int x = j * panel_size + 2 + startx + panel_size / 2;
        int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
        int status = (state.is_gameover() ? 7 : 4);
        panels->draw(x, y, type * PANEL_SIZE, status * PANEL_SIZE, PANEL_SIZE, PANEL_SIZE);
    }

    {
        int x = selector_x * panel_size - 2 + startx + panel_size / 2;
        int y = (selector_y + 1) * panel_size - 2 - offset + starty + panel_size / 2;
        selector->draw(x, y, 0, frames.selector * SELECTOR_GFX_HEIGHT / 2, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT / 2);
    }

    for (int i = 0; i < panel_table->height() + 1; i++)
    {
        for (int j = 0; j < panel_table->width(); j++)
        {
            const Panel& panel = panel_table->get(i, j);
            const Panel::State& state = panel.get_state();
            int x = j * panel_size + 2 + startx + panel_size / 2;
            int y = (i + 1) * panel_size + 2 - offset + starty + panel_size / 2;
            debug->draw(x, y, state.state_no() * 5, 0, 5, 10);
        }
    }
    border->draw(startx, starty);
    sf2d_end_frame();
}

void GameState::UpdateScore(int combo_num, int chain_num)
{
    score += CHAIN_VALUE[chain_num];
    score += 10 * combo_num + recursive_combo_score(combo_num, combo_num);
}
