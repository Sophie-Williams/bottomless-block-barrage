#ifndef TIME_WINDOW_HPP
#define TIME_WINDOW_HPP

#include <util/window.hpp>

class TimeWindow : public Window
{
public:
    TimeWindow();
    ~TimeWindow() {}
    void update() {}
    void draw();
private:
    // Starting of game
    u64 start_time;
};


#endif
