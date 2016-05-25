#ifndef ANIMATION_PARAMS_HPP
#define ANIMATION_PARAMS_HPP

#include "panel_table.hpp"

#define PANEL_SIZE 16

struct AnimationParams
{
    AnimationParams() : panel(0), selector(0), panel_counter(0), selector_counter(0) {}
    void Reset() {panel = selector = panel_counter = selector_counter = 0;}
    void Update(const PanelTable& state, bool is_danger);
    unsigned int panel;
    unsigned int selector;
    unsigned int panel_counter;
    unsigned int selector_counter;
};

#endif

