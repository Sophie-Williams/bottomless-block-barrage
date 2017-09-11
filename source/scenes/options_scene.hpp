#ifndef OPTIONS_SCENE_HPP
#define OPTIONS_SCENE_HPP

#include "scene.hpp"
#include "mode_select_scene.hpp"
#include <util/background.hpp>
#include <util/button.hpp>
#include <util/choice.hpp>
#include <util/command_window.hpp>
#include <util/image_selector.hpp>
#include <util/slider.hpp>
#include <util/text.hpp>
#include <util/texture.hpp>

struct GlobalOptions
{
    /// Panel graphics to use in modes
    std::string panel_gfx;
    /// Boolean for classic gameplay.  This will disable C-Links entirely.
    bool classic;
};

extern GlobalOptions global_config;

enum OptionItems
{
    OPTION_PANEL_GRAPHICS = 0,
    OPTION_CLASSIC_MODE = 1,
    OPTIONS_SIZE = 2,
};


class OptionsScene : public Scene2D
{
public:
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    void update_panel_graphics();
    void update_classic_mode();
    Text panels;
    ImageSelector panel_select;
    Text classic;
    Background menu_background_top;
    Background menu_background_bottom;
    int index;
};



#endif
