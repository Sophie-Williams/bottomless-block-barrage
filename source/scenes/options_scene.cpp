#include "options_scene.hpp"

#include "panels_gfx.hpp"

GlobalOptions global_config;

const std::string menu_background = "romfs:/graphics/menu/menu_background.png";

void OptionsScene::initialize()
{
    panels.create("Panels", 8, 12);

    panel_select.create(80, 8, 112, 16);
    int index = 0;
    int sub_index = 0;
    for (const auto& name_pdscr : panel_sets)
    {
        const auto& pdscr = name_pdscr.second;
        panel_select.add(pdscr.group, pdscr.gfx, &name_pdscr.first);
        if (global_config.panel_gfx == pdscr.gfx)
        {
            index = pdscr.index;
            sub_index = pdscr.sub_index;
        }
    }
    panel_select.set_selection(index);
    panel_select.set_sub_selection(sub_index);

    classic.create("Classic Gameplay", 8, 32);

    menu_background_top.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::TopScreen);
    menu_background_bottom.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::BottomScreen);
}

void OptionsScene::update()
{
    Scene2D::update();

    menu_background_top.update();
    menu_background_bottom.update();

    if (input.trigger(KEY_UP))
        index = (index - 1 + OPTIONS_SIZE) % OPTIONS_SIZE;
    if (input.trigger(KEY_DOWN))
        index = (index + 1) % OPTIONS_SIZE;
    if (input.trigger(KEY_B))
        current_scene = new ModeSelectScene();

    switch (index)
    {
        case OPTION_PANEL_GRAPHICS:
            update_panel_graphics();
            break;
        case OPTION_CLASSIC_MODE:
            update_classic_mode();
            break;
    }
}

void OptionsScene::draw_top()
{
    menu_background_top.draw();
    panels.draw();
    panel_select.draw();
}

void OptionsScene::draw_bottom()
{
    menu_background_bottom.draw();
}

void OptionsScene::update_panel_graphics()
{
    panel_select.update();
}

void OptionsScene::update_classic_mode()
{

}

