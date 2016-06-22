#include "animation_params.hpp"

void AnimationParams::update(int state, bool danger)
{
    static const std::vector<int> panel_frames = {0, 1, 2, 1, 0, 3};
    if (!danger)
        panel_counter = 0;

    if ((state == PanelTable::RISING && !danger) || state == PanelTable::RISED)
        panel = Panel::NORMAL;
    else if (state == PanelTable::GAMEOVER)
        panel = Panel::LOST;
    else if (state == PanelTable::CLOGGED)
        panel = Panel::UP;
    else if (state == PanelTable::RISING && danger)
    {
        panel_counter++;
        panel = panel_frames[(panel_counter >> 2) % panel_frames.size()];
    }

    selector_counter++;
    selector = (selector_counter >> 4) & 1;
}
