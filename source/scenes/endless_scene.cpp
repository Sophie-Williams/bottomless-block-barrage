#include "endless_scene.hpp"
#include "title_scene.hpp"
#include <algorithm>
#include <ctime>
#include <map>

#include "panels_gfx.h"
#include "border_gfx.h"
#include "selector_gfx.h"

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
            table = new PanelTable(11, 6, 6);
            break;
        case NORMAL:
            table = new PanelTable(11, 6, 6);
            break;
        case HARD:
            table = new PanelTable(11, 6, 6);
            break;
    }

    panel_table.reset(table);

    panels.reset(new Texture(panels_gfx, PANELS_GFX_WIDTH, PANELS_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    selector.reset(new Texture(selector_gfx, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));
    border.reset(new Texture(border_gfx, BORDER_GFX_WIDTH, BORDER_GFX_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));

    frames.Reset();
}

void EndlessScene::update()
{
    u32 trigger = hidKeysDown();
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

    panel_table->update(osGetTime() - last_frame, max_wait, held & KEY_R);

    frames.Update(*panel_table, panel_table->is_warning());
    last_frame = osGetTime();
    frame++;
}

void EndlessScene::draw_top_left()
{

}

void EndlessScene::draw_top_right()
{

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

    // Draw Selector
    {
        int x = selector_x * panel_size - 2 + startx + panel_size / 2;
        int y = (selector_y + 1) * panel_size - 2 - offset + starty + panel_size / 2;
        selector->draw(x, y, 0, frames.selector * SELECTOR_GFX_HEIGHT / 2, SELECTOR_GFX_WIDTH, SELECTOR_GFX_HEIGHT / 2);
    }

    border->draw(startx, starty);
}
