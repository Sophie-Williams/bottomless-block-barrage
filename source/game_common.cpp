#include "game_common.hpp"
#include <algorithm>
#include <map>

PanelSpeedSettings slow_speed_settings   = {8, 12, 8, 72, 36, 12, FALL_ANIMATION_FRAMES};
PanelSpeedSettings easy_speed_settings   = {4, 6, 4, 54, 27, 9, FALL_ANIMATION_FRAMES};
PanelSpeedSettings normal_speed_settings = {3, 5, 3, 40, 20, 6, FALL_ANIMATION_FRAMES};
PanelSpeedSettings hard_speed_settings   = {2, 4, 2, 27, 13, 4, FALL_ANIMATION_FRAMES};

const int CHAIN_VALUE[24] =
{
    0, 50, 80, 150, 300, 400,
    500, 700, 900, 1100, 1300, 1500,
    1800, 2100, 2400, 2700, 3000, 3400,
    3800, 4200, 4600, 5000, 5500, 6000
};

const int COMBO_VALUE[73] =
{
    0, 0, 0, 30, 60, 80, 90, 110, 140, 160, 210,
    230, 300, 320, 410, 430, 540, 560, 690, 710, 860,
    880, 1050, 1070, 1260, 1280, 1490, 1510, 1740, 1760, 2010,
    2030, 2300, 2320, 2610, 2630, 2940, 2960, 3290, 3310, 3660,
    3680, 4050, 4070, 4460, 4480, 4890, 4910, 5340, 5360, 5810,
    5830, 6300, 6320, 6810, 6830, 7340, 7360, 7890, 7910, 8460,
    8480, 9050, 9070, 9660, 9680, 10290, 10310, 10940, 10960, 11610,
    11630, 12300,
};

int calculate_score(int combo_num, int chain_num)
{
    return CHAIN_VALUE[chain_num] + COMBO_VALUE[combo_num];
}

int calculate_timeout(int combo, int chain, int difficulty, bool in_danger)
{
    int combo_time;
    int chain_time;
    if (!in_danger)
    {
        combo_time = combo / 2 + difficulty - 2;
        chain_time = difficulty + chain + 1;
    }
    else
    {
        combo_time = (combo + 10) * difficulty / 5 + 1;
        chain_time = difficulty * (1 + chain) + 1;
    }
    if (combo <= 3)
        combo_time = 0;
    if (chain <= 1)
        chain_time = 0;

    return std::min(20, std::max(combo_time, chain_time)) * 1000;
}

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

const std::map<int, int> level_table
{
    {0, 4},
    {10, 6},
    {20, 8},
    {30, 10},
    {40, 11},
    {50, 12},
    {60, 13},
    {70, 14},
    {80, 15},
    {90, 16},
    {100, -1}
};

int get_current_speed(int level)
{
    const auto& it = speed_table.find(level);
    if (it != speed_table.end())
        return it->second;

    int min = level - level % 10;
    int max = min + 10;
    if (min == 0) min = 1;

    int minv = speed_table.at(min);
    int maxv = speed_table.at(max);

    int diff = (maxv - minv) / 10;
    return minv + (level % 10) * diff;
}

int get_exp_to_level(int level)
{
    return level_table.at(level - level % 10);
}

