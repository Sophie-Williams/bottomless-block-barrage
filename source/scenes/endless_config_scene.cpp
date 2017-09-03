#include "endless_config_scene.hpp"
#include "mode_select_scene.hpp"
#include "endless_scene.hpp"

#include "panels_gfx.hpp"

const std::string menu_background = "romfs:/graphics/menu/menu_background.png";

void EndlessConfigScene::initialize()
{
    difficulty_choices.create(0, 0, 6 * 16, 16, 3, {"Easy", "Normal", "Hard"});
    difficulty_choices.set_active(true);
    difficulty_choices.set_selection(saved_config.difficulty);
    difficulty_choices.set_color(0, 0xFFE0E080);
    difficulty_choices.set_color(1, 0xFF58B8B8);
    difficulty_choices.set_color(2, 0xFFD098D0);

    const int y = difficulty_choices.window_height() + 4;
    level_text.create("Level", 0, y);
    level_slider.create(1, 100, 1, 48, y + 8, 100, 4);
    level_slider.set_value(saved_config.level);

    panel_text.create("Panel", 0, y + 20);
    panel_select.create(48, y + 20, 112, 16);

    int index = 0;
    int sub_index = 0;
    for (const auto& name_pdscr : panel_sets)
    {
        const auto& pdscr = name_pdscr.second;
        panel_select.add(pdscr.group, pdscr.gfx, &name_pdscr.first);
        if (saved_config.panel_gfx == pdscr.gfx)
        {
            index = pdscr.index;
            sub_index = pdscr.sub_index;
        }
    }
    panel_select.set_selection(index);
    panel_select.set_sub_selection(sub_index);

    start_button.create("romfs:/graphics/menu/start.png", 0, 0);
    start_button.centerx(BOTTOM_SCREEN_WIDTH);
    start_button.set_y(BOTTOM_SCREEN_HEIGHT - start_button.get_height() - 32);

    menu_background_top.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::TopScreen);
    menu_background_bottom.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::BottomScreen);
}

void EndlessConfigScene::update()
{
    Scene::update();
    difficulty_choices.update();
    level_slider.update();
    menu_background_top.update();
    menu_background_bottom.update();
    panel_select.update();
    start_button.update();

    if (input.trigger(KEY_START))
    {
        EndlessScene::GameConfig config;
        config.difficulty = (Difficulty) difficulty_choices.selection();
        config.level = level_slider.get_value();
        config.panel_gfx = *static_cast<const std::string*>(panel_select.client_data());
        current_scene = new EndlessScene(config);
        return;
    }

    if (difficulty_choices.is_active())
        update_difficulty_select();
    else if (level_slider.is_active())
        update_level_select();
    else if (panel_select.is_active())
        update_panel_select();
    else if (start_button.is_active())
        update_press_start();
}

void EndlessConfigScene::update_difficulty_select()
{
    if (input.trigger(KEY_A))
    {
        difficulty_choices.set_active(false);
        level_slider.set_active(true);
    }
    else if (input.trigger(KEY_B))
        current_scene = new ModeSelectScene();
}

void EndlessConfigScene::update_level_select()
{
    if (input.trigger(KEY_A))
    {
        panel_select.set_active(true);
        level_slider.set_active(false);
    }
    else if (input.trigger(KEY_B))
    {
        difficulty_choices.set_active(true);
        level_slider.set_active(false);
    }
}

void EndlessConfigScene::update_panel_select()
{
    if (input.trigger(KEY_A))
    {
        panel_select.set_active(false);
        start_button.set_active(true);
    }
    else if (input.trigger(KEY_B))
    {
        panel_select.set_active(false);
        level_slider.set_active(true);
    }
}

void EndlessConfigScene::update_press_start()
{
    if (input.trigger(KEY_A))
    {
        EndlessScene::GameConfig config;
        config.difficulty = (Difficulty) difficulty_choices.selection();
        config.level = level_slider.get_value();
        config.panel_gfx = *static_cast<const std::string*>(panel_select.client_data());
        current_scene = new EndlessScene(config);
        return;
    }
    else if (input.trigger(KEY_B))
    {
        panel_select.set_active(true);
        start_button.set_active(false);
    }
}

void EndlessConfigScene::draw_top()
{
    menu_background_top.draw();
}

void EndlessConfigScene::draw_bottom()
{
    menu_background_bottom.draw();
    difficulty_choices.draw();
    level_text.draw();
    level_slider.draw();
    panel_text.draw();
    panel_select.draw();
    start_button.draw();
}
