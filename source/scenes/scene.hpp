#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <string>
#include <util/font.hpp>
#include <util/hid_helper.hpp>
#include <util/text.hpp>
#include <audio.hpp>

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
    virtual void draw() {}
    virtual std::string music() {return get_track("Select.brstm");}
protected:
    KeyRepeatStore repeat;
};

class Scene2D : public Scene
{
public:
    Scene2D() {}
    ~Scene2D() {}
    void draw() override;
protected:
    virtual void draw_top() {}
    virtual void draw_bottom() {}
};

class Scene3D : public Scene
{
public:
    Scene3D() {}
    ~Scene3D() {}
    void draw() override;
protected:
    virtual void draw_top_left() {}
    virtual void draw_top_right() {}
    virtual void draw_bottom() {}
};
extern Scene* current_scene;

#endif
