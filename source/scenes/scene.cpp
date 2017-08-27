#include "scene.hpp"

#include <sf2d.h>

void Scene2D::draw()
{
    sf2d_start_render();
        sf2d_set_render_screen(GFX_TOP, GFX_LEFT);
        draw_top();
        sf2d_set_render_screen(GFX_BOTTOM, GFX_LEFT);
        draw_bottom();
    sf2d_end_render();
}

void Scene3D::draw()
{
    sf2d_start_render();
        sf2d_set_render_screen(GFX_TOP, GFX_LEFT);
        draw_top_left();
        sf2d_set_render_screen(GFX_TOP, GFX_RIGHT);
        draw_top_right();
        sf2d_set_render_screen(GFX_BOTTOM, GFX_LEFT);
        draw_bottom();
    sf2d_end_render();
}
