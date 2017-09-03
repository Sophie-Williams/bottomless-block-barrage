#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include "panel.hpp"
#include "panel_table.hpp"

#define RECORDER_MAJOR_VERSION 0
#define RECORDER_MINOR_VERSION 2

/**
 * This class records all relevant inputs to a game.
 * Things such as input per frame, initial board state,
 * and all next blocks generated are recorded.
 */
class Recorder
{
public:
    /// Saves this recorders state to file.
    bool save();
    /// Test only, saves recorder state to stream.
    bool save(std::ostream& file);
    /**
     * @brief settings
     * Sets the game settings for replay.
     * @param rows Number of rows for panel table.
     * @param columns Number of columns for panel table.
     * @param type Game type.
     * @param difficulty Difficulty setting.
     * @param level Speed level.
     */
    void settings(int rows, int columns, PanelTable::Type type, int difficulty, int level);
    /**
     * @brief set_initial
     * Sets the initial board
     * @param board Initial board consisting of columns * rows Panel types.
     */
    void set_initial(const std::vector<Panel::Type>& board);
    /**
     * @brief add_next
     * Adds a next set of panels.
     * @param next Next set of columns panels.
     */
    void add_next(const std::vector<Panel::Type>& next);
    /**
     * @brief add_input
     * Adds input for a frame.
     * @param trigger Current buttons newly pressed this frame.
     * @param held Current buttons pressed this frame.
     */
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
