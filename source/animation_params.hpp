#ifndef ANIMATION_PARAMS_HPP
#define ANIMATION_PARAMS_HPP

#include "panel_table.hpp"

struct AnimationParams
{
    AnimationParams() : panel(0), selector(0), panel_counter(0), selector_counter(0) {}
    void reset() {panel = selector = panel_counter = selector_counter = 0;}
    void update(int state, int& danger_panel);
    unsigned int panel;
    unsigned int selector;
    unsigned int panel_counter;
    unsigned int selector_counter;
};

int get_panel_frame(const Panel& panel, int def, int danger_panel, bool danger, bool gameover);

#endif

