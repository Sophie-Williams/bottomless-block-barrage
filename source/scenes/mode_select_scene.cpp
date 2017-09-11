#include "mode_select_scene.hpp"
#include <util/window.hpp>
#include "title_scene.hpp"
#include "endless_config_scene.hpp"
#include "score_config_scene.hpp"
#include "puzzle_select_scene.hpp"
#include "options_scene.hpp"

const std::string menu_background = "romfs:/graphics/menu/menu_background.png";

///TODO Have all text stored in romfs for translation support.
const char* help_text[MODES_SIZE] =
{
    "Play until you lose!\nThe longer you play,\nthe higher the speed!",
    "Play with a certain time,\nlines, or move limit.\nTry to score as high as\npossible!",
    "Clear all of the panels in a certain number of\nmoves. 120 puzzles are\nincluded!",
    "",
    "",
    "",
    "Change game options\nhere!"
};

void ModeSelectScene::initialize()
{
    command_window.create(0, 0,  7 * 16, 16, 1, {"Endless", "Score", "Puzzle", "Mission", "Versus", "Online", "Options"});
    command_window.center(BOTTOM_SCREEN_WIDTH, BOTTOM_SCREEN_WIDTH);
    command_window.set_active(true);
    command_window.set_color(MISSION, 0xFF808080);
    command_window.set_color(VERSUS, 0xFF808080);
    command_window.set_color(ONLINE, 0xFF808080);
    command_window.set_color(OPTIONS, 0xFF808080);

    help_window.create(0, 0, TOP_SCREEN_WIDTH / 3 - WINDOW_BORDER_SIZE, TOP_SCREEN_HEIGHT - WINDOW_BORDER_SIZE * 2, 10);
    help_window.set_active(true);
    help_window.set_text(help_text[0]);

    menu_background_top.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::TopScreen);
    menu_background_bottom.create(menu_background, -1, 1, Background::Autoscroll | Background::Repeating | Background::BottomScreen);
}

void ModeSelectScene::update()
{
    Scene2D::update();
    command_window.update();
    help_window.update();
    menu_background_top.update();
    menu_background_bottom.update();


    /// TODO implement events so hacks like this aren't necessary
    if (mode != command_window.selection())
    {
        mode = command_window.selection();
        help_window.set_text(help_text[mode]);
    }

    if (input.trigger(KEY_A))
    {
        switch (command_window.selection())
        {
            case ENDLESS:
                current_scene = new EndlessConfigScene();
                break;
            case SCORE:
                current_scene = new ScoreConfigScene();
                break;
            case PUZZLE:
                current_scene = new PuzzleSelectScene();
                break;
            case MISSION:
            case VERSUS:
            case ONLINE:
            case OPTIONS:
                break;
        }
    }
    else if (input.trigger(KEY_B))
        current_scene = new TitleScene();
}

void ModeSelectScene::draw_top()
{
    menu_background_top.draw();
    help_window.draw();
}

void ModeSelectScene::draw_bottom()
{
    menu_background_bottom.draw();
    command_window.draw();
}
