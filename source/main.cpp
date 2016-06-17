#include <3ds.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <sf2d.h>

#include <scenes/scene.hpp>
#include <scenes/title_scene.hpp>
#include <util/unnamed.hpp>

Scene* current_scene = NULL;

int main()
{
    romfsInit();
    sf2d_init();
    sf2d_set_clear_color(RGBA8(0x0, 0x0, 0x0, 0xFF));
    sf2d_set_3D(0);
    unnamed_init();

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

        sf2d_swapbuffers();
    }

    unnamed_exit();
    sf2d_fini();
    romfsExit();

    return 0;
}

