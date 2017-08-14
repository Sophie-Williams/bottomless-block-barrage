#ifndef INFO_WINDOW_HPP
#define INFO_WINDOW_HPP

#include <util/window.hpp>
#include <util/bar.hpp>
#include <scenes/game_scene.hpp> // For Difficulty
#include "panel_table.hpp"

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
    void set_next(int exp, int exp_req) {next_exp_bar.set(exp, exp_req);}
    void set_timeout(int t) {timeout = t;}
    void set_panel_table(PanelTable* panel_table) {table = panel_table;}
private:
    // Not owned
    PanelTable* table = nullptr;
    Bar next_exp_bar;
    Bar time_left_bar;
    // Starting of game
    u64 start_time;
    int score;
    int level;
    int timeout;
    Difficulty difficulty;
};

#endif
