#include "game_common.hpp"
#include <algorithm>
#include <map>

PanelSpeedSettings slow_speed_settings   = {8, 12, 8, 72, 36, 12, FALL_ANIMATION_FRAMES, false};
PanelSpeedSettings easy_speed_settings   = {4, 6, 4, 66, 33, 11, FALL_ANIMATION_FRAMES, false};
PanelSpeedSettings normal_speed_settings = {3, 5, 3, 45, 25, 9, FALL_ANIMATION_FRAMES, false};
PanelSpeedSettings hard_speed_settings   = {2, 4, 2, 33, 17, 6, FALL_ANIMATION_FRAMES, false};

const int CHAIN_VALUE[24] =
{
    0, 50, 80, 150, 300, 400,
    500, 700, 900, 1100, 1300, 1500,
    1800, 2100, 2400, 2700, 3000, 3400,
    3800, 4200, 4600, 5000, 5500, 6000
};

/// TODO verify these values.
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

// Per speed level this is the value to add to a counter on each frame.
// Once the counter is >= 0x1000 another counter represented the panels rise amount in pixels (hence called rise-counter)
// is incremented and 0x1000 is subtracted from the counter.
// Once the rise amount is 16 a rise happens and the rise-counter is reset to 0.
const std::map<int, int> speed_table
{
    {1, 0x0047},
    {2, 0x0044},
    {3, 0x0050},
    {4, 0x0055},
    {5, 0x0059},
    {6, 0x0061},
    {7, 0x0068},
    {8, 0x0070},
    {9, 0x0077},
    {10, 0x0084},
    {11, 0x0090},
    {12, 0x009a},
    {13, 0x00af},
    {14, 0x00bb},
    {15, 0x00c8},
    {16, 0x00d6},
    {17, 0x00e4},
    {18, 0x00f3},
    {19, 0x0104},
    {20, 0x0115},
    {21, 0x0128},
    {22, 0x013c},
    {23, 0x0151},
    {24, 0x0168},
    {25, 0x0181},
    {26, 0x019c},
    {27, 0x01b7},
    {28, 0x01d7},
    {29, 0x01fc},
    {30, 0x0226},
    {31, 0x0253},
    {32, 0x0288},
    {33, 0x02c0},
    {34, 0x02fa},
    {35, 0x0333},
    {36, 0x0381},
    {37, 0x03d2},
    {38, 0x0410},
    {39, 0x0469},
    {40, 0x04bd},
    {41, 0x051e},
    {42, 0x0572},
    {43, 0x05f4},
    {44, 0x0666},
    {45, 0x06eb},
    {46, 0x0750},
    {47, 0x07c1},
    {48, 0x0842},
    {49, 0x08d3},
    {50, 0x0924},
    {51, 0x097b},
    {52, 0x09d8},
    {53, 0x0a3d},
    {54, 0x0aaa},
    {55, 0x0b21},
    {56, 0x0ba2},
    {57, 0x0c30},
    {58, 0x0ccc},
    {59, 0x0d79},
    {60, 0x0e38},
    {61, 0x0f0f},
    {62, 0x1000},
}

// Number of Panels to match for next level
const std::map<int, int> level_table
{
    {1, 9},
    {2, 12},
    {7, 14},
    {9, 16},
    {11, 24},
    {16, 22},
    {17, 20},
    {18, 18},
    {19, 16},
    {21, 36},
    {31, 39},
    {40, 45},
};
