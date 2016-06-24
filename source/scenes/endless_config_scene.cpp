#include "endless_config_scene.hpp"
#include "title_scene.hpp"
#include "endless_scene.hpp"

#include "menu_background_gfx.h"
#include "panels_gfx.hpp"


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
    level_slider.create(1, 100, 1, 48, y + 4, 100, 4);
    level_slider.set_value(saved_config.level);

    panel_text.create("Panel", 0, y + 20);
    panel_select.create(48, y + 20, 112, 16);
    int id = 0;
    for (const auto& name_pdscr : panel_sets)
    {
        const auto& pdscr = name_pdscr.second;
        int pitch = pdscr.panel_size * 7 + (pdscr.include_unmatchable ? pdscr.panel_size : 0);
        panel_select.add(pdscr.gfx, pitch);
        index_to_id[id] = name_pdscr.first;
        id++;
    }

    menu_background_top.create(menu_background_gfx, MENU_BACKGROUND_GFX_WIDTH, MENU_BACKGROUND_GFX_HEIGHT,
                               -1, 1, Background::Autoscroll | Background::Repeating | Background::TopScreen);
    menu_background_bottom.create(menu_background_gfx, MENU_BACKGROUND_GFX_WIDTH, MENU_BACKGROUND_GFX_HEIGHT,
                                  -1, 1, Background::Autoscroll | Background::Repeating | Background::BottomScreen);
}

void EndlessConfigScene::update()
{
    difficulty_choices.update();
    level_slider.update();
    menu_background_top.update();
    menu_background_bottom.update();
    panel_select.update();

    if (hidKeysDown() & KEY_START)
    {
        EndlessScene::Config config;
        config.difficulty = (Difficulty) difficulty_choices.selection();
        config.level = level_slider.get_value();
        config.panel_gfx = index_to_id[panel_select.selection()];
        current_scene = new EndlessScene(config);
        return;
    }

    if (difficulty_choices.is_active())
        update_difficulty_select();
    else if (level_slider.is_active())
        update_level_select();
    else if (panel_select.is_active())
        update_panel_select();
}

void EndlessConfigScene::update_difficulty_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        difficulty_choices.set_active(false);
        level_slider.set_active(true);
    }
    else if (trigger & KEY_B)
        current_scene = new TitleScene();
}

void EndlessConfigScene::update_level_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        panel_select.set_active(true);
        level_slider.set_active(false);
    }
    else if (trigger & KEY_B)
    {
        difficulty_choices.set_active(true);
        level_slider.set_active(false);
    }
}

void EndlessConfigScene::update_panel_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        EndlessScene::Config config;
        config.difficulty = (Difficulty) difficulty_choices.selection();
        config.level = level_slider.get_value();
        config.panel_gfx = index_to_id[panel_select.selection()];
        current_scene = new EndlessScene(config);
        return;
    }
    else if (trigger & KEY_B)
    {
        panel_select.set_active(false);
        level_slider.set_active(true);
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
}
