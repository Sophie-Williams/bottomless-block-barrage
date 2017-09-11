#include "animation_params.hpp"

enum FrameGraphic
{
    NO_DRAW = -1,
    NORMAL = 0,
    SLIGHT_UP = 1,
    UP = 2,
    DOWN = 3,
    PENDING = 4,
    MATCH = 5,
    BLINK = 6,
    LOST = 7,
    GRAPHICS_SIZE = 8,
};

int get_panel_frame(const Panel& panel, int def, int danger_panel, bool danger, bool gameover)
{
    static const std::vector<int> panel_fell_frames = {3, 2, 1};
    int frame = def;

    if (gameover)
        frame = LOST;
    else if (panel.is_bottom())
        frame = PENDING;
    else if (panel.is_pending_match())
        frame = (panel.get_countdown() & 1) ? BLINK : NORMAL;
    else if (panel.is_matched())
        frame = MATCH;
    else if (panel.is_removed())
        frame = NO_DRAW;
    else if (panel.is_falling() || panel.is_fall_end())
        frame = NORMAL;
    else if (panel.is_fell_idle())
        frame = panel_fell_frames[(FALL_ANIMATION_FRAMES - panel.get_countdown()) / FALL_ANIMATION_DELAY];
    else if (danger)
        frame = danger_panel;

    return frame;
}

void AnimationParams::update(int state, int& danger_panel)
{
    static const std::vector<int> panel_frames = {0, 1, 2, 1, 0, 3};

    if (state == PanelTable::RISING || state == PanelTable::RISED)
        panel = NORMAL;
    else if (state == PanelTable::GAMEOVER)
        panel = LOST;
    else if (state == PanelTable::CLOGGED)
        panel = UP;

    panel_counter++;
    danger_panel = panel_frames[(panel_counter >> 2) % panel_frames.size()];

    selector_counter++;
    selector = (selector_counter >> 4) & 1;
}
