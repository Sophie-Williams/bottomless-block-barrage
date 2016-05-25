#include <map>
#include <cstdio>
#include <ctime>

const int CHAIN_VALUE[24] =
{
    0, 50, 80, 150, 300, 400,
    500, 700, 900, 1100, 1300, 1500,
    1800, 2100, 2400, 2700, 3000, 3400,
    3800, 4200, 4600, 5000, 5500, 6000
};

int recursive_combo_score(int combo, int combonum)
{
    int scored = 0;
    if (combo > 3 && combo <= 5)
        scored = 10 * (combonum - 2);
    else if (combo > 5)
        scored = 10 * (combonum - combo - 1) + recursive_combo_score(combo - 2, combonum);
    return scored;
}

/*void GameState::UpdateScore(int combo_num, int chain_num)
{
    score += CHAIN_VALUE[chain_num];
    score += 10 * combo_num + recursive_combo_score(combo_num, combo_num);
}*/
