#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>
#include <sf2d.h>
#include "widget.hpp"
#include "texture.hpp"

#define WINDOW_SKIN_WIDTH 128
#define WINDOW_SKIN_HEIGHT 128

#define WINDOW_BG_WIDTH 64
#define WINDOW_BG_HEIGHT 64

#define WINDOW_BG_X 0
#define WINDOW_BG_Y 0
#define WINDOW_DISABLED_BG_X 0
#define WINDOW_DISABLED_BG_Y 64

#define WINDOW_BORDER_WIDTH 32
#define WINDOW_BORDER_HEIGHT 32
#define WINDOW_BORDER_SIZE 8 // 8 x 8 px

#define WINDOW_BORDER_TL_X WINDOW_BG_WIDTH
#define WINDOW_BORDER_TL_Y 0
#define WINDOW_BORDER_TR_X (WINDOW_BG_WIDTH + WINDOW_BORDER_WIDTH - WINDOW_BORDER_SIZE)
#define WINDOW_BORDER_TR_Y 0
#define WINDOW_BORDER_BL_X WINDOW_BG_WIDTH
#define WINDOW_BORDER_BL_Y (WINDOW_BORDER_HEIGHT - WINDOW_BORDER_SIZE)
#define WINDOW_BORDER_BR_X (WINDOW_BG_WIDTH + WINDOW_BORDER_WIDTH - WINDOW_BORDER_SIZE)
#define WINDOW_BORDER_BR_Y (WINDOW_BORDER_HEIGHT - WINDOW_BORDER_SIZE)

#define WINDOW_DISABLED_BORDER_X 64
#define WINDOW_DISABLED_BORDER_Y 64

class WindowSkin
{
public:
    WindowSkin() {}
    ~WindowSkin() {}
    bool valid() const {return skin.valid();}
    void create(const std::string& path);
    void draw(int x, int y, int w, int h, float contents_opacity = 1.0f, float border_opacity = 1.0f);
private:
    Texture skin;
};

class Window : public Widget
{
public:
    Window() {}
    Window(int wx, int wy, int ww, int wh, int style = 0);
    void create(int wx, int wy, int ww, int wh, int style = 0);
    ~Window() {}
    virtual void update() {}
    virtual void draw();

    int window_width() const {return width + WINDOW_BORDER_SIZE * 2;}
    int window_height() const {return height + WINDOW_BORDER_SIZE * 2;}

    void draw_text(const std::string& str, int wx, int wy, u32 color = 0xFFFFFFFF);
    void draw_text(const std::string& str, int wx, int wy, int ww, int wh, u32 color = 0xFFFFFFFF, Alignment align = LEFT_CENTER);

    void set_contents_opacity(float opacity) {contents_opacity = opacity;}
    void set_border_opacity(float opacity) {border_opacity = opacity;}

    static void set_skin(const std::string& path);
    static WindowSkin skin;
private:
    float contents_opacity = 1.0f;
    float border_opacity = 1.0f;
};

#endif
