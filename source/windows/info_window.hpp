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
    void update() {}
    void draw();
    void set_score(int s) {score = s;}
    void set_level(int l) {level = l;}
    void set_experience(int exp, int exp_req)
    {
        speed_exp.set_value(exp);
        speed_exp.set_max(exp_req);
    }
private:
    Bar speed_exp;
    u64 start_time;
    int score;
    int level;
    const Difficulty difficulty;
};

#endif
