#ifndef ENDLESS_SCENE_HPP
#define ENDLESS_SCENE_HPP

#include "game_scene.hpp"
#include <windows/info_window.hpp>
#include <windows/ccc_window.hpp>

class EndlessScene : public GameScene
{
public:
    EndlessScene(const Config& c) : GameScene(c) {}
    ~EndlessScene() {}
protected:
    void init_menu();

    void update_end_match();
    void update_on_timeout();
    void update_on_matched();

    void update_windows();

    void draw_game_top();
private:
    InfoWindow info;
    CCCWindow ccc_stats;
};

#endif
