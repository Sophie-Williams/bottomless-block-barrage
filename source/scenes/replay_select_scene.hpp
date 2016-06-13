#ifndef REPLAY_SELECT_SCENE_HPP
#define REPLAY_SELECT_SCENE_HPP

#include "scene.hpp"
#include <util/command_window.hpp>
#include <string>
#include <vector>


class ReplaySelectScene : public Scene
{
public:
    void initialize();
    void update();
protected:
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();
private:
    CommandWindow replays;
};


#endif

