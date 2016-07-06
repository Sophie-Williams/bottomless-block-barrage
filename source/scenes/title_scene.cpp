#include "title_scene.hpp"
#include <util/window.hpp>
#include "mode_select_scene.hpp"
#include "replay_select_scene.hpp"
#include "version.hpp"

void TitleScene::initialize()
{
    background.create("romfs:/graphics/menu/title_screen.png");
    press_start.create("romfs:/graphics/menu/press_start.png");
    version.create(version_str, 0, 0);
    version.set_x(BOTTOM_SCREEN_WIDTH - version.get_width());
}

void TitleScene::update()
{
    Scene::update();

    u32 trigger = hidKeysDown();
    u32 held = hidKeysHeld();

    if (held == (KEY_L | KEY_R | KEY_START))
        current_scene = new ReplaySelectScene();
    else if (trigger & KEY_START)
        current_scene = new ModeSelectScene();
    else if (trigger & KEY_B)
        current_scene = NULL;
}

void TitleScene::draw_top()
{
    background.draw();
}

void TitleScene::draw_bottom()
{
    press_start.draw((BOTTOM_SCREEN_WIDTH - press_start.width()) / 2,
                     (BOTTOM_SCREEN_HEIGHT - press_start.height()) / 2);
    version.draw();
}
