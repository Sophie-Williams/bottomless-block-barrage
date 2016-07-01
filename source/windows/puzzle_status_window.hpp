#ifndef PUZZLE_STATUS_WINDOW_HPP
#define PUZZLE_STATUS_WINDOW_HPP

#include <string>
#include <util/window.hpp>

class PuzzleStatusWindow : public Window
{
public:
    PuzzleStatusWindow(const std::string& stage);
    void update() {}
    void draw();
    void set_moves(int m) {moves = m;}
private:
    const std::string stage;
    int moves;
};


#endif

