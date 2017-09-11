#ifndef SCORE_SCENE_HPP
#define SCORE_SCENE_HPP

#include "game_scene.hpp"
#include <3ds.h>
#include <windows/score_info_window.hpp>

class ScoreScene : public GameScene
{
public:
    ScoreScene(const GameConfig& c) : GameScene(c) {}
    void initialize() override;
    void update() override;

protected:
    void init_menu() override;
    void init_panel_table() override;

    bool is_gameover() const override;

    void update_windows() override;
    void update_gameover() override;
    void update_on_gameover() override;

    void draw_game_top() override;
    void draw_game_bottom() override;
    void draw_gameover_top() override;

private:
    ScoreInfoWindow info;
    Text game_over;
    CommandWindow try_again_command;
    int time;
    u64 last_frame;
};

#endif
