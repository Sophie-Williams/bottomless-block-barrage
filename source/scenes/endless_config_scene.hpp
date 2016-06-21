#ifndef ENDLESS_CONFIG_SCENE_HPP
#define ENDLESS_CONFIG_SCENE_HPP

#include "scene.hpp"
#include "endless_scene.hpp"
#include <util/texture.hpp>
#include <util/background.hpp>
#include <util/command_window.hpp>
#include <util/slider.hpp>
#include <util/text.hpp>

class EndlessConfigScene : public Scene
{
public:
    EndlessConfigScene(const EndlessScene::Config& config = EndlessScene::Config()) : saved_config(config) {}
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    void update_difficulty_select();
    void update_level_select();
    EndlessScene::Config saved_config;

    CommandWindow difficulty_choices;
    Text level_text;
    Slider level_slider;
    Background menu_background_top;
    Background menu_background_bottom;
};

#endif

