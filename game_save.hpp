#ifndef GAME_SAVE_HPP
#define GAME_SAVE_HPP

struct EndlessStats
{
    u32 score;
    u32 time_taken;
};

struct PuzzleStats
{
    u32 set_id;
    u32 stage;
    u32 level;
    u32 time_taken;
};

struct TimeAttackStats
{
    u32 mode;
    u32 score;
};

struct GameSave
{
    EndlessStats endless_high_scores;
    std::vector<TimeAttackStats> time_attack_high_scores;
    std::vector<PuzzleStat> solved_puzzles;
};

void load_game_state()
void save_game_state()

#endif
