#ifndef GAME_COMMON_HPP
#define GAME_COMMON_HPP

#include "panel.hpp"

extern PanelSpeedSettings slow_speed_settings;
extern PanelSpeedSettings easy_speed_settings;
extern PanelSpeedSettings normal_speed_settings;
extern PanelSpeedSettings hard_speed_settings;

int calculate_score(int combo_num, int chain_num);
int calculate_timeout(int combo, int chain, int difficulty, bool in_danger);
int get_current_speed(int level);
int get_exp_to_level(int level);

#endif
