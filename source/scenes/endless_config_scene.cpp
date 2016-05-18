#include "endless_config_scene.hpp"
#include "title_scene.hpp"

void EndlessConfigScene::initialize()
{
    difficulty_choices.reset(new CommandWindow(0, 0,  6 * 16, {"Easy", "Normal", "Hard"}));
    level_slider.reset(new Slider(1, 50, 25, 80 + 8, 56, 100, 4));
    level_slider->set_active(true);
}

void EndlessConfigScene::update()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
    }
    else if (trigger & KEY_START)
    {

    }
    else if (trigger & KEY_B)
        current_scene = new TitleScene();
}

void EndlessConfigScene::draw_top_left()
{
    difficulty_choices->draw();
    level_slider->draw();
    font->draw("Level", 0, 56);
}

void EndlessConfigScene::draw_top_right()
{
    difficulty_choices->draw();
    level_slider->draw();
    font->draw("Level", 0, 56);
}

void EndlessConfigScene::draw_bottom()
{

}
