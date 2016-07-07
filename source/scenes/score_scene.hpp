#ifndef SCORE_SCENE_HPP
#define SCORE_SCENE_HPP

#include "game_scene.hpp"
#include <3ds.h>
#include <windows/score_info_window.hpp>

class ScoreScene : public GameScene
{
public:
    ScoreScene(const GameConfig& c) : GameScene(c) {}
    ~ScoreScene() {}
protected:
    void init_menu();
    void init_panel_table();

    bool is_gameover() const;

    void update_end_match();
    void update_on_timeout();
    void update_on_matched();
    void update_on_gameover();

    void update_windows();
    void update_gameover();

    void draw_game_top();
    void draw_gameover_top();
private:
    ScoreInfoWindow info;

    Text game_over;
    CommandWindow try_again_command;
    int time;
};

#endif
