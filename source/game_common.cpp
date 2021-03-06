#include "game_common.hpp"
#include <algorithm>
#include <map>

PanelSpeedSettings slow_speed_settings   = {3, 11, 1, 54, 30, 10, FALL_ANIMATION_FRAMES};
PanelSpeedSettings easy_speed_settings   = {3, 11, 1, 46, 25, 9, FALL_ANIMATION_FRAMES}; // 22 + 1 * 2, 22 + 3, 8 + 1
PanelSpeedSettings normal_speed_settings = {3, 11, 1, 38, 20, 8, FALL_ANIMATION_FRAMES}; // 18 + 1 * 2, 17 + 3, 7 + 1
PanelSpeedSettings hard_speed_settings   = {3, 11, 1, 30, 15, 7, FALL_ANIMATION_FRAMES}; // 14 + 1 * 2, 12 + 3, 6 + 1

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

int combo_timeout[31][3] = {
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x078, 0x05a, 0x03c},
 {0x096, 0x078, 0x05a},
 {0x0b4, 0x096, 0x078},
 {0x0d2, 0x0b4, 0x096},
 {0x0f0, 0x0d2, 0x0b4},
 {0x10e, 0x0f0, 0x0d2},
 {0x12c, 0x10e, 0x0f0},
 {0x14a, 0x12c, 0x10e},
 {0x168, 0x14a, 0x12c},
 {0x168, 0x168, 0x14a},
 {0x1a4, 0x168, 0x168},
 {0x1c2, 0x1a4, 0x168},
 {0x1e0, 0x1c2, 0x1a4},
 {0x1fe, 0x1e0, 0x1c2},
 {0x21c, 0x1fe, 0x1e0},
 {0x23a, 0x21c, 0x1fe},
 {0x258, 0x23a, 0x21c},
 {0x276, 0x258, 0x23a},
 {0x294, 0x276, 0x258},
 {0x2b2, 0x294, 0x276},
 {0x2d0, 0x2b2, 0x294},
 {0x2ee, 0x2d0, 0x2b2},
 {0x30c, 0x2ee, 0x2d0},
 {0x32a, 0x30c, 0x2ee},
 {0x348, 0x32a, 0x30c},
 {0x366, 0x348, 0x32a},
 {0x384, 0x366, 0x348},
};

int combo_danger_timeout[31][3] = {
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},

 {0x258, 0x1a4, 0x0f0},
 {0x258, 0x1a4, 0x0f0},
 {0x258, 0x1a4, 0x0f0},
 {0x258, 0x1a4, 0x0f0},
 {0x258, 0x1a4, 0x0f0},

 {0x30c, 0x21c, 0x12c},
 {0x30c, 0x21c, 0x12c},

 {0x3c0, 0x294, 0x168},
 {0x3c0, 0x294, 0x168},
 {0x3c0, 0x294, 0x168},
 {0x3c0, 0x294, 0x168},
 {0x3c0, 0x294, 0x168},

 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
 {0x3c0, 0x30c, 0x1a4},
};

int chain_timeout[15][3] = {
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x12c, 0x0e1, 0x096},
 {0x168, 0x11d, 0x0d2},
 {0x1a4, 0x159, 0x10e},
 {0x1e0, 0x195, 0x14a},
 {0x21c, 0x1d1, 0x186},
 {0x258, 0x20d, 0x1c2},
 {0x294, 0x249, 0x1fe},
 {0x2d0, 0x285, 0x23a},
 {0x30c, 0x2c1, 0x276},
 {0x348, 0x2fd, 0x2b2},
 {0x384, 0x339, 0x2ee},
 {0x3c0, 0x375, 0x32a},
 {0x3c0, 0x375, 0x32a},
};

int chain_danger_timeout[15][3] = {
 {0x000, 0x000, 0x000},
 {0x000, 0x000, 0x000},
 {0x258, 0x1a4, 0x0f0},
 {0x30c, 0x21c, 0x12c},
 {0x3c0, 0x294, 0x168},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
 {0x474, 0x30c, 0x1a4},
};

// Per speed level this is the value to add to a counter on each frame.
// Once the counter is >= 0x1000 another counter represented the panels rise amount in pixels (hence called rise-counter)
// is incremented and 0x1000 is subtracted from the counter.
// Once the rise amount is 16 a rise happens and the rise-counter is reset to 0.
const std::map<int, int> speed_table
{
    {1,   0x0047},
    {2,   0x0044},
    {3,   0x0050},
    {4,   0x0055},
    {5,   0x0059},
    {6,   0x0061},
    {7,   0x0068},
    {8,   0x0070},
    {9,   0x0077},
    {10,  0x0084},
    {11,  0x0090},
    {12,  0x009a},
    {13,  0x00af},
    {14,  0x00bb},
    {15,  0x00c8},
    {16,  0x00d6},
    {17,  0x00e4},
    {18,  0x00f3},
    {19,  0x0104},
    {20,  0x0115},
    {21,  0x0128},
    {22,  0x013c},
    {23,  0x0151},
    {24,  0x0168},
    {25,  0x0181},
    {26,  0x019c},
    {27,  0x01b7},
    {28,  0x01d7},
    {29,  0x01fc},
    {30,  0x0226},
    {31,  0x0253},
    {32,  0x0288},
    {33,  0x02c0},
    {34,  0x02fa},
    {35,  0x0333},
    {36,  0x0381},
    {37,  0x03d2},
    {38,  0x0410},
    {39,  0x0469},
    {40,  0x04bd},
    {41,  0x051e},
    {42,  0x0572},
    {43,  0x05f4},
    {44,  0x0666},
    {45,  0x06eb},
    {46,  0x0750},
    {47,  0x07c1},
    {48,  0x0842},
    {49,  0x08d3},
    {50,  0x0924},
    {51,  0x097b},
    {52,  0x09d8},
    {53,  0x0a3d},
    {54,  0x0aaa},
    {55,  0x0b21},
    {56,  0x0ba2},
    {57,  0x0c30},
    {58,  0x0ccc},
    {59,  0x0d79},
    {60,  0x0e38},
    {61,  0x0f0f},
    {100, 0x1000},
};

// Number of Panels to match for next level
const std::map<int, int> level_table
{
    {1,    9},
    {6,   12},
    {8,   14},
    {10,  16},
    {15,  24},
    {16,  22},
    {17,  20},
    {18,  18},
    {20,  16},
    {30,  36},
    {31,  39},
    {100, 45},
};

int get_speed_for_level(int level)
{
    return speed_table.lower_bound(level)->second;
}

int get_panels_for_level(int level)
{
    return level_table.lower_bound(level)->second;
}

int calculate_timeout(int combo, int chain, int difficulty, bool warning)
{
    int timeout1 = warning ? combo_danger_timeout[combo][difficulty] : combo_timeout[combo][difficulty];
    int timeout2 = warning ? chain_danger_timeout[chain][difficulty] : chain_timeout[chain][difficulty];

    return std::max(timeout1, timeout2);
}
