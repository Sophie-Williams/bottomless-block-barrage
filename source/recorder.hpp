#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include "panel.hpp"
#include "panel_table.hpp"

/**
 * This class records all relevant inputs to a game.
 * Things such as input per frame, initial board state,
 * and all next blocks generated are recorded.
 */
class Recorder
{
public:
    bool save();
    bool save(std::ostream& file);
    void settings(int rows, int columns, PanelTable::Type type, int difficulty, int level);
    void set_initial(const std::vector<Panel::Type>& board);
    void add_next(const std::vector<Panel::Type>& next);
    void add_input(unsigned int trigger, unsigned int held);
private:
    struct Input
    {
        Input(unsigned int t = 0, unsigned int h = 0) : trigger(t), held(h), frames(1) {}
        unsigned int trigger;
        unsigned int held;
        unsigned int frames;
    };

    int rows = 11;
    int columns = 6;
    PanelTable::Type type = PanelTable::Type::ENDLESS;
    int difficulty = 1;
    int level = 1;
    std::vector<Panel::Type> initial;
    std::vector<Panel::Type> next;
    std::vector<Input> input;

};

#endif
