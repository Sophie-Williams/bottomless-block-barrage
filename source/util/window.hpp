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
    void create(const void *src_buffer, sf2d_texfmt pixel_format, sf2d_place place);
    void draw(int x, int y, int w, int h);
private:
    Texture skin;
};

class Window : public Widget
{
public:
    Window() {}
    Window(int wx, int wy, int ww, int wh, int style = 0);
    void create(int wx, int wy, int ww, int wh, int style = 0);
    virtual ~Window() {}
    virtual void update() {}
    virtual void draw();
    void draw_text(int wx, int wy, const std::string& str);
    static void set_skin(const void *src_buffer, sf2d_texfmt pixel_format, sf2d_place place);
    static WindowSkin skin;

};

#endif
