#ifndef ENDLESS_CONFIG_SCENE_HPP
#define ENDLESS_CONFIG_SCENE_HPP

#include "scene.hpp"
#include "endless_scene.hpp"
#include <util/background.hpp>
#include <util/button.hpp>
#include <util/command_window.hpp>
#include <util/image_selector.hpp>
#include <util/slider.hpp>
#include <util/text.hpp>
#include <util/texture.hpp>

class EndlessConfigScene : public Scene
{
public:
    EndlessConfigScene(const EndlessScene::GameConfig& config = EndlessScene::GameConfig()) : saved_config(config) {}
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    void update_difficulty_select();
    void update_level_select();
    void update_panel_select();
    void update_press_start();
    EndlessScene::GameConfig saved_config;

    Background menu_background_top;
    Background menu_background_bottom;
    CommandWindow difficulty_choices;
    Text level_text;
    Slider level_slider;
    Text panel_text;
    ImageSelector panel_select;
    Button start_button;
};

#endif

