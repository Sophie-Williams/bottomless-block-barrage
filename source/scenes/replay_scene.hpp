#ifndef REPLAY_SCENE_HPP
#define REPLAY_SCENE_HPP

#include "game_scene.hpp"
#include "replay_helpers.hpp"
#include <windows/info_window.hpp>
#include <windows/ccc_window.hpp>

class ReplayScene : public GameScene
{
public:
    ReplayScene(const GameConfig& c) : GameScene(c) {}
    void initialize() override;
protected:
    void init_panel_table() override;
    void init_recorder() override {}
    void init_menu() override;

    bool is_gameover() const override {return table->is_gameover();}
    void update_input() override;
    void update_on_matched() override;
    void update_windows() override;
    void update_on_level() override;
    void update_gameover() override;

    void draw_game_top() override;
    void draw_game_bottom() override;
private:
    InfoWindow info;
    CCCWindow ccc_stats;
    ReplayInfo replay_info;
};

#endif
