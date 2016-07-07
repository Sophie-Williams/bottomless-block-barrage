#include "puzzle_select_scene.hpp"
#include "mode_select_scene.hpp"
#include "puzzle_scene.hpp"

#include <cstdio>
#include <util/file_helper.hpp>

const std::string menu_background = "romfs:/graphics/menu/menu_background.png";

std::map<std::string, PuzzleSet> PuzzleSelectScene::puzzles;

void PuzzleSelectScene::initialize()
{
    menu_background_top.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::TopScreen);
    menu_background_bottom.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::BottomScreen);

    std::vector<std::string> sets;
    for (const auto& set : puzzles)
        sets.push_back(set.first);

    set_choices.create(0, 0, 48, 16, 1, sets);
    set_choices.set_active(true);

    stage_choices.create(set_choices.get_width() + 16, 0, 80, 16, 1, {});
    stage_choices.set_hidden(true);

    level_choices.create(set_choices.get_width() + stage_choices.get_width() + 32, 0, 80, 16, 1, {});
    level_choices.set_hidden(true);
}

void PuzzleSelectScene::update()
{
    Scene::update();
    set_choices.update();
    stage_choices.update();
    level_choices.update();
    menu_background_bottom.update();
    menu_background_top.update();

    if (set_choices.is_active())
        update_set_select();
    else if (stage_choices.is_active())
        update_stage_select();
    else if (level_choices.is_active())
        update_level_select();
}

void PuzzleSelectScene::update_set_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        std::vector<std::string> names = puzzles[set_choices.choice()].get_stages();
        set_choices.set_active(false);
        stage_choices.set_active(true);
        stage_choices.set_hidden(false);
        stage_choices.set_choices(names);
        stage_choices.set_height(names.size() * 16);
    }
    else if (trigger & KEY_B)
    {
        current_scene = new ModeSelectScene();
    }
}

void PuzzleSelectScene::update_stage_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        std::vector<std::string> names = puzzles[set_choices.choice()].stages[stage_choices.choice()].levels;
        stage_choices.set_active(false);
        level_choices.set_active(true);
        level_choices.set_hidden(false);
        level_choices.set_choices(names);
        level_choices.set_height(names.size() * 16);
    }
    else if (trigger & KEY_B)
    {
        set_choices.set_active(true);
        stage_choices.set_active(false);
        stage_choices.set_hidden(true);
    }
}

void PuzzleSelectScene::update_level_select()
{
    u32 trigger = hidKeysDown();
    if (trigger & KEY_A || trigger & KEY_START)
    {
        PuzzleScene::GameConfig config;
        config.puzzle_filename = construct_puzzle_filename(set_choices.choice(), stage_choices.choice(), level_choices.choice());
        config.set_name = set_choices.choice();
        config.stage_id = stage_choices.selection();
        config.level_id = level_choices.selection();
        config.level_name = level_choices.choice();
        current_scene = new PuzzleScene(config);
    }
    else if (trigger & KEY_B)
    {
        stage_choices.set_active(true);
        level_choices.set_active(false);
        level_choices.set_hidden(true);
    }
}

void PuzzleSelectScene::draw_top()
{
    menu_background_top.draw();
}

void PuzzleSelectScene::draw_bottom()
{
    menu_background_bottom.draw();
    set_choices.draw();
    stage_choices.draw();
    level_choices.draw();
}
