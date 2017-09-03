#include "score_config_scene.hpp"
#include "mode_select_scene.hpp"
#include "score_scene.hpp"

#include "panels_gfx.hpp"

const std::string menu_background = "romfs:/graphics/menu/menu_background.png";

const std::vector<std::vector<std::string>> type_choices_values =
{
    {"1 minute", "2 minutes", "5 minutes", "10 minutes"},
    {"10 lines", "25 lines", "50 lines", "100 lines"},
    {"50 moves", "100 moves", "250 moves", "500 moves"},
};

const std::vector<std::map<int, int>> time_mode_to_index = {
    {{60, 0}, {120, 1}, {300, 2}, {600, 3}},
    {{10, 0}, {25, 1}, {50, 2}, {100, 3}},
    {{50, 0}, {100, 1}, {250, 2}, {500, 3}},
};

const std::vector<std::vector<int>> index_to_time_mode {
    {60, 120, 300, 600},
    {10, 25, 50, 100},
    {50, 100, 250, 500},
};

void ScoreConfigScene::initialize()
{
    type_choices.create(0, 0, 6 * 16, 16, 3, {"Time", "Lines", "Moves"});
    type_choices.set_active(true);
    type_choices.set_selection(saved_config.time_mode);

    length_text.create("Length", 0, 36);
    number_choices.create(48, 36, 64, 16, type_choices_values[saved_config.time_mode]);
    number_choices.set_selection(time_mode_to_index[saved_config.time_mode].at(saved_config.value));

    difficulty_choices.create(0, 56, 6 * 16, 16, 3, {"Easy", "Normal", "Hard"});
    difficulty_choices.set_selection(saved_config.difficulty);
    difficulty_choices.set_color(0, 0xFFE0E080);
    difficulty_choices.set_color(1, 0xFF58B8B8);
    difficulty_choices.set_color(2, 0xFFD098D0);

    level_text.create("Level", 0, 92);
    level_slider.create(1, 100, 1, 48, 96, 100, 4);
    level_slider.set_value(saved_config.level);

    panel_text.create("Panel", 0, 112);
    panel_select.create(48, 112, 112, 16);

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

void ScoreConfigScene::update()
{
    Scene2D::update();
    type_choices.update();
    number_choices.update();
    difficulty_choices.update();
    level_slider.update();
    menu_background_top.update();
    menu_background_bottom.update();
    panel_select.update();
    start_button.update();

    if (input.trigger(KEY_START))
    {
        ScoreScene::GameConfig config;
        config.time_mode = (TimingMode) type_choices.selection();
        config.value = index_to_time_mode[type_choices.selection()][number_choices.selection()];
        config.difficulty = (Difficulty) difficulty_choices.selection();
        config.level = level_slider.get_value();
        config.panel_gfx = *static_cast<const std::string*>(panel_select.client_data());
        current_scene = new ScoreScene(config);
        return;
    }

    if (type_choices.is_active())
        update_type_select();
    else if (number_choices.is_active())
        update_number_select();
    else if (difficulty_choices.is_active())
        update_difficulty_select();
    else if (level_slider.is_active())
        update_level_select();
    else if (panel_select.is_active())
        update_panel_select();
    else if (start_button.is_active())
        update_press_start();
}

void ScoreConfigScene::update_type_select()
{
    if (input.trigger(KEY_A))
    {
        type_choices.set_active(false);
        number_choices.set_choices(type_choices_values[type_choices.selection()]);
        number_choices.set_active(true);
    }
    else if (input.trigger(KEY_B))
        current_scene = new ModeSelectScene();
}

void ScoreConfigScene::update_number_select()
{
    if (input.trigger(KEY_A))
    {
        number_choices.set_active(false);
        difficulty_choices.set_active(true);
    }
    else if (input.trigger(KEY_B))
    {
        type_choices.set_active(true);
        number_choices.set_active(false);
    }
}

void ScoreConfigScene::update_difficulty_select()
{
    if (input.trigger(KEY_A))
    {
        level_slider.set_active(true);
        difficulty_choices.set_active(false);
    }
    else if (input.trigger(KEY_B))
    {
        number_choices.set_active(true);
        difficulty_choices.set_active(false);
    }
}

void ScoreConfigScene::update_level_select()
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

void ScoreConfigScene::update_panel_select()
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

void ScoreConfigScene::update_press_start()
{
    if (input.trigger(KEY_A))
    {
        ScoreScene::GameConfig config;
        config.time_mode = (TimingMode) type_choices.selection();
        config.value = index_to_time_mode[type_choices.selection()][number_choices.selection()];
        config.difficulty = (Difficulty) difficulty_choices.selection();
        config.level = level_slider.get_value();
        config.panel_gfx = *static_cast<const std::string*>(panel_select.client_data());
        current_scene = new ScoreScene(config);
        return;
    }
    else if (input.trigger(KEY_B))
    {
        start_button.set_active(false);
        panel_select.set_active(true);
    }
}

void ScoreConfigScene::draw_top()
{
    menu_background_top.draw();
}

void ScoreConfigScene::draw_bottom()
{
    menu_background_bottom.draw();
    type_choices.draw();
    length_text.draw();
    number_choices.draw();
    difficulty_choices.draw();
    level_text.draw();
    level_slider.draw();
    panel_text.draw();
    panel_select.draw();
    start_button.draw();
}
