#include "endless_config_scene.hpp"
#include "title_scene.hpp"
#include "endless_scene.hpp"

void EndlessConfigScene::initialize()
{
    difficulty_choices.reset(new CommandWindow(0, 0, 8 * 16, 16, 3, {"Easy", "Normal", "Hard"}));
    difficulty_choices->set_active(true);
    level_slider.reset(new Slider(1, 100, 1, 80 + 8, 24, 100, 4));
}

void EndlessConfigScene::update()
{
    difficulty_choices->update();
    level_slider->update();

    if (difficulty_choices->is_active())
        update_difficulty_select();
    else if (level_slider->is_active())
        update_level_select();
}

void EndlessConfigScene::update_difficulty_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        difficulty_choices->set_active(false);
        level_slider->set_active(true);
    }
    else if (trigger & KEY_START)
    {
        EndlessScene::Config config;
        config.difficulty = (Difficulty) difficulty_choices->selection();
        config.level = level_slider->get_value();
        current_scene = new EndlessScene(config);
    }
    else if (trigger & KEY_B)
        current_scene = new TitleScene();
}

void EndlessConfigScene::update_level_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A || trigger & KEY_START)
    {
        EndlessScene::Config config;
        config.difficulty = (Difficulty) difficulty_choices->selection();
        config.level = level_slider->get_value();
        current_scene = new EndlessScene(config);
    }
    else if (trigger & KEY_B)
    {
        difficulty_choices->set_active(true);
        level_slider->set_active(false);
    }
}

void EndlessConfigScene::draw_top_left()
{
    difficulty_choices->draw();
    level_slider->draw();
    font->draw("Level", 0, 24);
}

void EndlessConfigScene::draw_top_right()
{
    difficulty_choices->draw();
    level_slider->draw();
    font->draw("Level", 0, 24);
}

void EndlessConfigScene::draw_bottom()
{

}
