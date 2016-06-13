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
    replays.create(0, 0, TOP_SCREEN_WIDTH / 3, 16, 3, get_replays());
    replays.set_active(true);
}

void ReplaySelectScene::update()
{
    replays.update();

    u32 trigger = hidKeysHeld();
    if (trigger & KEY_A)
    {
        std::string choice = replays.choice();
        choice = "/bbb-moves/" + choice;
        ReplayScene::Config config;
        config.replay_filename = choice;
        current_scene = new ReplayScene(config);
    }
    else if (trigger & KEY_B || (trigger == 0 && replays.empty()))
    {
        current_scene = new TitleScene();
    }

}

void ReplaySelectScene::draw_top_left()
{
    replays.draw();
}

void ReplaySelectScene::draw_top_right()
{
    replays.draw();
}

void ReplaySelectScene::draw_bottom()
{

}
