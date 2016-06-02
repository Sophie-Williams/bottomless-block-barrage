#ifndef CCC_WINDOW
#define CCC_WINDOW

#include <util/window.hpp>
#include "panel_table.hpp"

class CCCWindow : public Window
{
public:
    CCCWindow();
    ~CCCWindow() {}
    void update() {}
    void draw();
    void set_matchinfo(const MatchInfo& match_info) {info = match_info;}
private:
    MatchInfo info;
};

#endif
