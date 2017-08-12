#ifndef GAME_COMMON_HPP
#define GAME_COMMON_HPP

#include "panel.hpp"

#define SELECTOR_REPEAT_MS 200
#define SELECTOR_QUICK_MS 50

extern PanelSpeedSettings slow_speed_settings;
extern PanelSpeedSettings easy_speed_settings;
extern PanelSpeedSettings normal_speed_settings;
extern PanelSpeedSettings hard_speed_settings;

int calculate_score(int combo_num, int chain_num);
int calculate_timeout(int combo, int chain, int difficulty, bool in_danger);
int get_speed_for_level(int level);
int get_panels_for_level(int level);

#endif
