#ifndef PUZZLE_SELECT_SCENE_HPP
#define PUZZLE_SELECT_SCENE_HPP

#include "scene.hpp"
#include <util/texture.hpp>
#include <util/command_window.hpp>

class PuzzleSelectScene : public Scene
{
public:
    void initialize();
    void update();
protected:
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();
private:
    void update_set_select();
    void update_stage_select();
    void update_level_select();
    Texture background;
    CommandWindow set_choices;
    CommandWindow stage_choices;
    CommandWindow level_choices;
};

#endif
