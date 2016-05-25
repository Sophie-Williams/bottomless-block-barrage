#include "animation_params.hpp"

void AnimationParams::Update(const PanelTable& table, bool danger)
{
    static const std::vector<int> panel_frames = {0, 1, 2, 1, 0, 3};
    if (!danger)
        panel_counter = 0;

    if ((table.is_rising() && !danger) || table.is_rised() || table.is_stopped() || table.is_stopping())
        panel = 0;
    else if (table.is_gameover())
        panel = 7;
    else if (table.is_clogged())
        panel = 2;
    else if (table.is_rising() && danger)
    {
        panel_counter++;
        panel = panel_frames[(panel_counter >> 2) % panel_frames.size()];
    }

    selector_counter++;
    selector = (selector_counter >> 4) & 1;
}
