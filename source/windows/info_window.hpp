#ifndef INFO_WINDOW_HPP
#define INFO_WINDOW_HPP

#include <util/window.hpp>
#include <util/bar.hpp>

enum Difficulty
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
};

class InfoWindow : public Window
{
public:
    InfoWindow(int level, Difficulty difficulty);
    ~InfoWindow() {}
    void update();
    void draw();
    void set_score(int s) {score = s;}
    void set_level(int l) {level = l;}
    void set_experience(int exp, int exp_req) {speed_exp_bar.set(exp, exp_req);}
    void set_timeout(int timeout);
    void clear_timeout();
    void start_timeout_timer();
    void pause_timeout_timer();
private:
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
    const Difficulty difficulty;
};

#endif
