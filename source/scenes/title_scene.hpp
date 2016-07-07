#ifndef TITLE_SCENE_HPP
#define TITLE_SCENE_HPP

#include "scene.hpp"
#include <util/button.hpp>
#include <util/texture.hpp>
#include <util/text.hpp>

class TitleScene : public Scene
{
public:
    void initialize();
    void update();
protected:
    void draw_top();
    void draw_bottom();
private:
    Texture background;
    Button press_start;
    Text version;
};

#endif
