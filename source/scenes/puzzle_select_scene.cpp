#include "puzzle_select_scene.hpp"
// TODO Get a better title screen.
#include "generic_title_screen.h"
#include "title_scene.hpp"
#include <util/window.hpp>
#include <cstdio>

static std::vector<std::string> get_level_choices()
{
    std::vector<std::string> ret;
    char buf[128];
    for (unsigned int i = 0; i < 10; i++)
    {
        sprintf(buf, "Level %d", i + 1);
        ret.push_back(buf);
    }

    return ret;
}

static std::vector<std::string> get_stage_choices()
{
    std::vector<std::string> ret;
    char buf[128];
    for (unsigned int i = 0; i < 10; i++)
    {
        sprintf(buf, "Stage %d", i + 1);
        ret.push_back(buf);
    }

    return ret;
}

void PuzzleSelectScene::initialize()
{
    ///TODO Implement a resource manager so this isn't done multiple times.
    background.reset(new Texture(generic_title_screen, GENERIC_TITLE_SCREEN_WIDTH, GENERIC_TITLE_SCREEN_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM));

    set_choices.reset(new CommandWindow(0, 0, 5 * 16, 16, 1, {"TA", "TA-EX"}));
    set_choices->set_active(true);

    stage_choices.reset(new CommandWindow(80, 0, 8 * 16, 16, 1, {}));
    stage_choices->set_hidden(true);

    level_choices.reset(new CommandWindow(80 + 128, 0, 8 * 16, 16, 1, {}));
    level_choices->set_hidden(true);
}

void PuzzleSelectScene::update()
{
    set_choices->update();
    stage_choices->update();
    level_choices->update();

    if (set_choices->is_active())
        update_set_select();
    else if (stage_choices->is_active())
        update_stage_select();
    else if (level_choices->is_active())
        update_level_select();
}

void PuzzleSelectScene::update_set_select()
{
    u32 trigger = hidKeysDown();

    if (trigger & KEY_A)
    {
        set_choices->set_active(false);
        stage_choices->set_active(true);
        stage_choices->set_hidden(false);
        stage_choices->set_choices(get_stage_choices());
        stage_choices->set_height(10 * 16);
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
        stage_choices->set_active(false);
        level_choices->set_active(true);
        level_choices->set_hidden(false);
        level_choices->set_choices(get_level_choices());
        level_choices->set_height(10 * 16);
    }
    else if (trigger & KEY_B)
    {
        set_choices->set_active(true);
        stage_choices->set_active(false);
        stage_choices->set_hidden(true);
    }
}

void PuzzleSelectScene::update_level_select()
{
    u32 trigger = hidKeysDown();
    if (trigger & KEY_A)
    {
    }
    else if (trigger & KEY_B)
    {
        stage_choices->set_active(true);
        level_choices->set_active(false);
        level_choices->set_hidden(true);
    }
}

void PuzzleSelectScene::draw_top_left()
{
    set_choices->draw();
    stage_choices->draw();
    level_choices->draw();
}

void PuzzleSelectScene::draw_top_right()
{
    set_choices->draw();
    stage_choices->draw();
    level_choices->draw();
}

void PuzzleSelectScene::draw_bottom()
{
}
