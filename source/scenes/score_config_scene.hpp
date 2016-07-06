#ifndef SCORE_CONFIG_SCENE_HPP
#define SCORE_CONFIG_SCENE_HPP

#include "scene.hpp"
#include "score_scene.hpp"
#include <util/texture.hpp>
#include <util/background.hpp>
#include <util/choice.hpp>
#include <util/command_window.hpp>
#include <util/image_selector.hpp>
#include <util/slider.hpp>
#include <util/text.hpp>

class ScoreConfigScene : public Scene
{
public:
    ScoreConfigScene(const ScoreScene::Config& config = ScoreScene::Config()) : saved_config(config) {}
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    void update_type_select();
    void update_number_select();
    void update_difficulty_select();
    void update_level_select();
    void update_panel_select();
    ScoreScene::Config saved_config;

    CommandWindow type_choices;
    Text length_text;
    Choice number_choices;
    CommandWindow difficulty_choices;
    Text level_text;
    Slider level_slider;
    Text panel_text;
    ImageSelector panel_select;
    Background menu_background_top;
    Background menu_background_bottom;
};

#endif

