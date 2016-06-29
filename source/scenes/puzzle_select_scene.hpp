#ifndef PUZZLE_SELECT_SCENE_HPP
#define PUZZLE_SELECT_SCENE_HPP

#include "scene.hpp"

#include <map>
#include <string>
#include <vector>
#include <util/background.hpp>
#include <util/command_window.hpp>

struct PuzzleStage
{
    PuzzleStage() {}
    PuzzleStage(const std::string& n) : name(n) {}
    std::string name;
    std::vector<std::string> levels;
};

struct PuzzleSet
{
    PuzzleSet() {}
    PuzzleSet(const std::string& n) : name(n) {}
    std::vector<std::string> get_stages() const;
    std::string name;
    std::map<std::string, PuzzleStage> stages;
};

class PuzzleSelectScene : public Scene
{
public:
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    void update_set_select();
    void update_stage_select();
    void update_level_select();
    std::string construct_filename();
    std::map<std::string, PuzzleSet> set_files;
    CommandWindow set_choices;
    CommandWindow stage_choices;
    CommandWindow level_choices;
    Background menu_background_top;
    Background menu_background_bottom;
};

#endif
