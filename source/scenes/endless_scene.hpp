#ifndef ENDLESS_SCENE_HPP
#define ENDLESS_SCENE_HPP

#include "game_scene.hpp"
#include <memory>
#include <windows/info_window.hpp>

class EndlessScene : public GameScene
{
public:
    EndlessScene(const GameConfig& c) : GameScene(c) {}
    ~EndlessScene() {}
protected:
    void init_menu();

    void update_on_matched();
    void update_on_gameover();
    void update_on_level();
    void update_on_timeout();

    void update_windows();
    void update_gameover();

    void draw_game_top();
    void draw_gameover_top();
private:
    InfoWindow info;

    Text game_over;
    Text try_again;
    CommandWindow save_replay_command;
    CommandWindow try_again_command;
};

#endif
