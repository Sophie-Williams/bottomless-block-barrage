#include "replay_select_scene.hpp"
#include "replay_scene.hpp"
#include "title_scene.hpp"

extern "C"
{
#include <dirent.h>
}

std::vector<std::string> get_replays()
{
    std::vector<std::string> files;
    struct dirent* dir;
    DIR* d = opendir("/bbb-moves");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            files.push_back(dir->d_name);
        }
        closedir(d);
    }

    std::sort(files.begin(), files.end());
    return files;
}

void ReplaySelectScene::initialize()
{
    replays.create(0, 0, TOP_SCREEN_WIDTH - 2 * WINDOW_BORDER_SIZE, 16, 1, get_replays());
    replays.set_active(true);
}

void ReplaySelectScene::update()
{
    Scene::update();
    replays.update();

    if (input.trigger(KEY_A))
    {
        std::string choice = replays.choice();
        choice = "/bbb-moves/" + choice;
        ReplayScene::GameConfig config;
        config.replay_filename = choice;
        current_scene = new ReplayScene(config);
    }
    else if (input.trigger(KEY_B) || (input.trigger() == 0 && replays.empty()))
    {
        current_scene = new TitleScene();
    }

}

void ReplaySelectScene::draw_top()
{
    replays.draw();
}

void ReplaySelectScene::draw_bottom()
{

}
