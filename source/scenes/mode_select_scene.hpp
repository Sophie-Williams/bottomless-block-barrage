#ifndef MODE_SELECT_SCENE_HPP
#define MODE_SELECT_SCENE_HPP

#include "scene.hpp"
#include <util/background.hpp>
#include <util/command_window.hpp>
#include <util/texture.hpp>
#include <util/text.hpp>
#include <windows/help_window.hpp>

enum
{
    ENDLESS = 0,
    SCORE = 1,
    PUZZLE = 2,
    MISSION = 3,
    VERSUS = 4,
    ONLINE = 5,
    OPTIONS = 6,
    MODES_SIZE = 7,
};

class ModeSelectScene : public Scene2D
{
public:
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    CommandWindow command_window;
    HelpWindow help_window;
    Background menu_background_top;
    Background menu_background_bottom;
    int mode = 0;

};

#endif
