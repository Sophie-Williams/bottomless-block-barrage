#ifndef REPLAY_SCENE_HPP
#define REPLAY_SCENE_HPP

#include "game_scene.hpp"
#include <windows/info_window.hpp>
#include <windows/ccc_window.hpp>

class ReplayScene : public GameScene
{
public:
    ReplayScene(const GameConfig& c) : GameScene(c) {}
    ~ReplayScene() {}
protected:
    void init_recorder();
    void init_menu();

    bool is_gameover() const {return false;}
    void update_input();
    void update_on_matched();
    void update_windows();
    void update_on_level();

    void draw_game_top();
    void draw_game_bottom();
private:
    InfoWindow info;
    CCCWindow ccc_stats;
    u32 held = 0;
    u32 trigger = 0;
};

#endif
