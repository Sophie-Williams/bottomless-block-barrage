#ifndef HELP_WINDOW_HPP
#define HELP_WINDOW_HPP

#include <util/window.hpp>
#include <3ds.h>

class HelpWindow : public Window
{
public:
    HelpWindow() : last_frame_ms(0), speed_ms(0), pos(0) {}
    HelpWindow(int x, int y, int width, int height, u64 speed);
    void create(int x, int y, int width, int height, u64 speed);
    void set_text(const std::string& str);
    void update();
    void draw();
private:
    std::string text;
    u64 last_frame_ms;
    u64 speed_ms;
    int pos;
};

#endif
