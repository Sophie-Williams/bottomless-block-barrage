#ifndef SCORE_INFO_WINDOW_HPP
#define SCORE_INFO_WINDOW_HPP

#include <util/window.hpp>
#include <util/bar.hpp>
#include <scenes/game_scene.hpp> // For Difficulty

class ScoreInfoWindow : public Window
{
public:
    ScoreInfoWindow();
    ~ScoreInfoWindow() {}
    void update();
    void draw();
    void set_mode(TimingMode tm) {mode = tm;}
    void set_value(u64 val) {value = val;}
    void set_difficulty(Difficulty diff) {difficulty = diff;}
    void set_score(int s) {score = s;}
    void set_level(int l) {level = l;}
    void set_experience(int exp, int exp_req) {speed_exp_bar.set(exp, exp_req);}
    void set_timeout(int timeout);
    void clear_timeout();
    void start_timeout_timer();
    void pause_timeout_timer();
private:
    TimingMode mode;
    u64 value;
    Bar speed_exp_bar;
    Bar time_left_bar;
    // Starting of game
    u64 start_time;
    // Last update
    u64 last_update;
    // Max timeout
    int timeout;
    // Timeout started
    bool timeout_started;
    int score;
    int level;
    Difficulty difficulty;
};

#endif
