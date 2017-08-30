#include "title_scene.hpp"
#include <util/window.hpp>
#include "mode_select_scene.hpp"
#include "replay_select_scene.hpp"
#include "version.hpp"

void TitleScene::initialize()
{
    background.create("romfs:/graphics/menu/title_screen.png");
    press_start.create("romfs:/graphics/menu/press_start.png", 0, 0);
    press_start.set_active(true);
    press_start.center(BOTTOM_SCREEN_WIDTH, BOTTOM_SCREEN_HEIGHT);
    version.create(version_str, 0, 0);
    version.set_x(BOTTOM_SCREEN_WIDTH - version.get_width());
}

void TitleScene::update()
{
    Scene::update();

    if (input->held(KEY_L) && input->held(KEY_R) && input->held(KEY_START))
        current_scene = new ReplaySelectScene();
    else if (input->trigger(KEY_START))
        current_scene = new ModeSelectScene();
    else if (input->trigger(KEY_B))
        current_scene = NULL;
}

void TitleScene::draw_top()
{
    background.draw();
}

void TitleScene::draw_bottom()
{
    press_start.draw();
    version.draw();
}
