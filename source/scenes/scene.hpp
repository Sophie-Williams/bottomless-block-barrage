#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <string>
#include <util/font.hpp>
#include <util/hid_helper.hpp>
#include <util/text.hpp>

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
    virtual void update();
    void draw();
protected:
    virtual void draw_top() {}
    virtual void draw_top_left() {draw_top();}
    virtual void draw_top_right() {draw_top();}
    virtual void draw_bottom() {}
    KeyRepeatStore repeat;
};

extern Scene* current_scene;

#endif
