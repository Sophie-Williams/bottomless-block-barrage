#include <3ds.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <sf2d.h>

#include <scenes/scene.hpp>
#include <scenes/title_scene.hpp>
#include <scenes/puzzle_select_scene.hpp>
#include "puzzle_set.hpp"
#include <util/file_helper.hpp>
#include <util/sfw.hpp>
/*#include <vgmstream_player.h>
extern "C"
{
    #include <vgmstream.h>
}*/

Scene* current_scene = NULL;

void get_official_puzzle_sets(std::map<std::string, PuzzleSet>& files)
{
    std::vector<std::string> sets = dir_entries("romfs:/puzzles");
    for (const auto& set : sets)
    {
        PuzzleSet puzzle_set(set);
        std::vector<std::string> stages = dir_entries("romfs:/puzzles/" + set, true);
        for (const auto& stage : stages)
        {
            PuzzleStage puzzle_stage(stage);
            puzzle_stage.levels = dir_filenames("romfs:/puzzles/" + set + "/" + stage, "bbb", false, true);
            puzzle_set.stages.emplace(stage, puzzle_stage);
        }
        puzzle_set.stage_names = stages;
        files.emplace(set, puzzle_set);
    }
}

int main()
{
    romfsInit();
    sf2d_init();
    sf2d_set_clear_color(RGBA8(0x0, 0x0, 0x0, 0xFF));
    sf2d_set_3D(0);
    sfw_init();
	//ndspInit();

    std::string current_music = "";
    get_official_puzzle_sets(PuzzleSelectScene::puzzles);
    std::unique_ptr<Scene> scene;
    current_scene = new TitleScene();

    while (aptMainLoop())
    {
        hidScanInput();

        if (current_scene != scene.get())
        {
            scene.reset(current_scene);
            if (!scene) break;
            scene->initialize();
        }

        scene->update();
        scene->draw();

        /*if (scene->music() != current_music)
        {
            current_music = scene->music();
            vgmstream_play(current_music, 0);
        }*/
    }

    //vgmstream_stop();
    //ndspExit();
    sfw_exit();
    sf2d_fini();
    romfsExit();

    return 0;
}

