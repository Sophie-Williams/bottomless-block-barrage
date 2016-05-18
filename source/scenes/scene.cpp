#include "scene.hpp"

#include <sf2d.h>

void Scene::draw()
{
    sf2d_start_frame(GFX_TOP, GFX_LEFT);
        draw_top_left();
    sf2d_end_frame();
    sf2d_start_frame(GFX_TOP, GFX_RIGHT);
        draw_top_right();
    sf2d_end_frame();
    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        draw_bottom();
    sf2d_end_frame();
}
