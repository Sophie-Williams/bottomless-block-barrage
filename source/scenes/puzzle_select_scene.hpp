#ifndef PUZZLE_SELECT_SCENE_HPP
#define PUZZLE_SELECT_SCENE_HPP

#include "scene.hpp"

#include <map>
#include <string>
#include <util/background.hpp>
#include <util/command_window.hpp>
#include "puzzle_set.hpp"

class PuzzleSelectScene : public Scene2D
{
public:
    void initialize();
    void update();
    static std::map<std::string, PuzzleSet> puzzles;
protected:
    void draw_top();
    void draw_bottom();
private:
    void update_set_select();
    void update_stage_select();
    void update_level_select();
    std::string construct_filename();
    CommandWindow set_choices;
    CommandWindow stage_choices;
    CommandWindow level_choices;
    Background menu_background_top;
    Background menu_background_bottom;
};

#endif
