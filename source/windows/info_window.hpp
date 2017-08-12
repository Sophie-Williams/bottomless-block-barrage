#ifndef INFO_WINDOW_HPP
#define INFO_WINDOW_HPP

#include <util/window.hpp>
#include <util/bar.hpp>
#include <scenes/game_scene.hpp> // For Difficulty

class InfoWindow : public Window
{
public:
    InfoWindow();
    ~InfoWindow() {}
    void update();
    void draw();
    void set_difficulty(Difficulty diff) {difficulty = diff;}
    void set_score(int s) {score = s;}
    void set_level(int l) {level = l;}
    void set_next(int exp, int exp_req) {speed_exp_bar.set(exp, exp_req);}
private:
    Bar speed_exp_bar;
    Bar time_left_bar;
    // Starting of game
    u64 start_time;
    int score;
    int level;
    Difficulty difficulty;
};

#endif
