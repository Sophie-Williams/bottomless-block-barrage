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
    void set_experience(int exp, int exp_req)
    {
        speed_exp_bar.set_value(exp);
        speed_exp_bar.set_max(exp_req);
    }
    void set_timeout(int timeout);
private:
    Bar speed_exp_bar;
    Bar time_left_bar;
    // Starting of game
    u64 start_time;
    // Starting of the timeout
    u64 start_timeout;
    int timeout;
    int score;
    int level;
    const Difficulty difficulty;
};

#endif
