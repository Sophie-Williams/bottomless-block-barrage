#include "puzzle_select_scene.hpp"
#include "menu_background_gfx.h"
#include "title_scene.hpp"
#include "puzzle_scene.hpp"

#include <cstdio>
#include <sstream>
#include <util/file_helper.hpp>


std::vector<std::string> PuzzleSet::get_stages() const
{
    std::vector<std::string> names;
    for (const auto& stage_info : stages)
        names.push_back(stage_info.first);
    return names;
}


void get_official_puzzle_sets(std::map<std::string, PuzzleSet>& files)
{
    std::vector<std::string> sets = dir_entries("romfs:/puzzles");
    for (const auto& set : sets)
    {
        PuzzleSet puzzle_set(set);
        std::vector<std::string> stages = dir_entries("romfs:/puzzles/" + set);
        for (const auto& stage : stages)
        {
            PuzzleStage puzzle_stage(stage);
            puzzle_stage.levels = dir_filenames("romfs:/puzzles/" + set + "/" + stage, "bbb", false);
            puzzle_set.stages.emplace(stage, puzzle_stage);
        }
        files.emplace(set, puzzle_set);
    }
}


void PuzzleSelectScene::initialize()
{
    ///TODO Implement a resource manager so this isn't done multiple times.
    menu_background_top.create(menu_background_gfx, MENU_BACKGROUND_GFX_WIDTH, MENU_BACKGROUND_GFX_HEIGHT,
                               -1, 1, Background::Autoscroll | Background::Repeating | Background::TopScreen);
    menu_background_bottom.create(menu_background_gfx, MENU_BACKGROUND_GFX_WIDTH, MENU_BACKGROUND_GFX_HEIGHT,
                                  -1, 1, Background::Autoscroll | Background::Repeating | Background::BottomScreen);

    get_official_puzzle_sets(set_files);
    std::vector<std::string> sets;
    for (const auto& set : set_files)
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
        std::vector<std::string> names = set_files[set_choices.choice()].get_stages();
        set_choices.set_active(false);
        stage_choices.set_active(true);
        stage_choices.set_hidden(false);
        stage_choices.set_choices(names);
        stage_choices.set_height(names.size() * 16);
    }
    else if (trigger & KEY_B)
    {
        current_scene = new TitleScene();
    }
}

void PuzzleSelectScene::update_stage_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        std::vector<std::string> names = set_files[set_choices.choice()].stages[stage_choices.choice()].levels;
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
        PuzzleScene::PuzzleConfig config;
        config.filename = construct_filename();
        config.stage_name = level_choices.choice();
        current_scene = new PuzzleScene(config);
    }
    else if (trigger & KEY_B)
    {
        stage_choices.set_active(true);
        level_choices.set_active(false);
        level_choices.set_hidden(true);
    }
}

std::string PuzzleSelectScene::construct_filename()
{
    std::stringstream str;
    str << "romfs:/puzzles/" << set_choices.choice() << "/" << stage_choices.choice() << "/";
    str << level_choices.choice() << ".bbb";
    return str.str();
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
