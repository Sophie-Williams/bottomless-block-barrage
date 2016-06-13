#ifndef TITLE_SCENE_HPP
#define TITLE_SCENE_HPP

#include "scene.hpp"
#include <util/texture.hpp>
#include <util/command_window.hpp>

enum
{
    ENDLESS = 0,
    SCORE = 1,
    PUZZLE = 2,
    MISSION = 3,
    VERSUS = 4,
};

class TitleScene : public Scene
{
public:
    void initialize();
    void update();
protected:
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();
private:
    Texture background;
    CommandWindow command_window;
    int mode;
};

#endif
