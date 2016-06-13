#ifndef ENDLESS_CONFIG_SCENE_HPP
#define ENDLESS_CONFIG_SCENE_HPP

#include "scene.hpp"
#include "endless_scene.hpp"
#include <util/texture.hpp>
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
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();
private:
    void update_difficulty_select();
    void update_level_select();
    EndlessScene::Config saved_config;

    CommandWindow difficulty_choices;
    Text level_text;
    Slider level_slider;
};

#endif

