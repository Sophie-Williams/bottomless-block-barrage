#include "animation_params.hpp"

void AnimationParams::update(int state, int& danger_panel)
{
    static const std::vector<int> panel_frames = {0, 1, 2, 1, 0, 3};

    if (state == PanelTable::RISING || state == PanelTable::RISED)
        panel = Panel::NORMAL;
    else if (state == PanelTable::GAMEOVER)
        panel = Panel::LOST;
    else if (state == PanelTable::CLOGGED)
        panel = Panel::UP;

    panel_counter++;
    danger_panel = panel_frames[(panel_counter >> 2) % panel_frames.size()];

    selector_counter++;
    selector = (selector_counter >> 4) & 1;
}
