#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <util/font.hpp>
#include <util/hid_helper.hpp>

#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240

extern std::unique_ptr<Font> font;

class Scene
{
public:
    Scene() {}
    virtual ~Scene() {}
    virtual void initialize() {}
    virtual void update() {}
    void draw();
protected:
    virtual void draw_top_left() {}
    virtual void draw_top_right() {}
    virtual void draw_bottom() {}
    KeyRepeatStore repeat;
private:
};

extern Scene* current_scene;

#endif
