#include "animation_params.hpp"

void AnimationParams::update(int state, bool danger)
{
    static const std::vector<int> panel_frames = {0, 1, 2, 1, 0, 3};
    if (!danger)
        panel_counter = 0;

    if ((state == PanelTable::RISING && !danger) || state == PanelTable::RISED || state == PanelTable::STOPPED)
        panel = 0;
    else if (state == PanelTable::GAMEOVER)
        panel = 7;
    else if (state == PanelTable::CLOGGED)
        panel = 2;
    else if (state == PanelTable::RISING && danger)
    {
        panel_counter++;
        panel = panel_frames[(panel_counter >> 2) % panel_frames.size()];
    }

    selector_counter++;
    selector = (selector_counter >> 4) & 1;
}
