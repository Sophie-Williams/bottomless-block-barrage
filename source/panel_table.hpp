#ifndef PANEL_TABLE_HPP
#define PANEL_TABLE_HPP

#include "panel.hpp"
#include "panel_source.hpp"

#include <list>
#include <memory>
#include <set>
#include <string>
#include <vector>

struct Point
{
    Point(int j, int i) : x(j), y(i) {}
    bool operator<(const Point& p) const {return (y != p.y) ? y > p.y : x > p.x;}
    bool operator==(const Point& p) const {return x == p.x && y == p.y;}
    int x;
    int y;
};

struct MatchInfo
{
    MatchInfo() : clink(0), chain(0), combo(0), swap_match(false), fall_match(false) {}
    bool empty() const {return clink == 0 && chain == 0 && combo == 0;}
    bool matched() const {return swap_match || fall_match;}
    bool is_combo() const {return combo > 3;}
    bool is_chain() const {return chain > 1;}
    bool is_clink() const {return clink > 0;}
    bool is_timeout() const {return combo > 3 || chain > 0;}
    std::string str() const;
    /** Panel x where match occurred */
    int x;
    /** Panel y where match occurred */
    int y;
    /** C-links (Chain links) are rapidly matching 3 or more blocks before they disappear */
    int clink;
    /** Chains are sequential matches after blocks are removed */
    int chain;
    /** Combos are matches of more than 3 blocks */
    int combo;
    /** If any panels was in state IDLE then this will be true (part of a clink) */
    bool swap_match;
    /** If any panels was in state END_FALL this will be true (part of a chain) */
    bool fall_match;
};

class PanelTable
{
public:
    enum Type
    {
        /// This type never rises.  The game is over when moves is 0 or all panels clear.
        MOVES = 0,
        /// This type will rise.  The game is over immediately when the panels have reached the top.
        ENDLESS = 1,
        /// This type will rise.  The game is over when the panels have reached the top after a delay.
        VERSUS = 2,
        /// This type will rise.  The game is over when a number of lines are fully cleared.
        NLINES = 3,
    };
    enum State
    {
        INVALID = 0,
        /// Puzzle mode panels will not rise.
        PUZZLE = 1,
        /// Panel Table is slowly rising.
        RISING = 2,
        /// Panel Table is being raised quickly (holding L/R).
        FAST_RISING = 3,
        /// Panel Table has risen by 1 blocks.  A new line of panels are generated here.
        RISED = 4,
        /// Dummy state to generate next blocks
        GENERATE_NEXT = 5,
        /// State for game over delay in Versus mode.
        CLOGGED = 6,
        /// Puzzle has been won. Or N lines have been cleared
        WIN = 7,
        /// Game over.
        GAMEOVER = 8,
    };

    struct Options
    {
        PanelSource* source;
        PanelSpeedSettings settings;
        Type type;
        int columns;
        int rows;
        int moves;
    };
    /// Create a new Panel Table
    PanelTable(const Options& opts);

    bool is_puzzle() const {return state == PUZZLE;}
    bool is_rising() const {return state == RISING;}
    bool is_fast_rising() const {return state == FAST_RISING;}
    bool is_rised() const {return state == RISED;}
    bool is_generate_next() const {return state == GENERATE_NEXT;}
    bool is_clogged() const {return state == CLOGGED;}
    bool is_gameover() const {return state == GAMEOVER;}
    bool is_win() const {return state == WIN;}

    const std::vector<Panel>& get_panels() const {return panels;}
    std::vector<Panel>& get_panels() {return panels;}
    const std::vector<Panel>& get_next() const {return next;}
    std::vector<Panel>& get_next() {return next;}
    Panel& get(int i, int j) {return panels[i * columns + j];}
    const Panel& get(int i, int j) const {return panels[i * columns + j];}
    Panel::Type value(int i, int j) const {return get(i, j).get_value();}
    bool empty(int i, int j) const {return get(i, j).empty();}
    bool special(int i, int j) const {return get(i, j).special();}
    bool matchable(int i, int j) const {return get(i, j).can_match();}
    bool normal(int i, int j) const {return get(i, j).normal();}

    void clear();
    /// Are the panels high
    bool warning() const;

    /// Have the panels reached the top row.
    bool danger() const;

    /// Returns a boolean for each column if they are very close to the top.
    std::vector<bool> danger_columns() const;

    /// Swaps the panel at i, j with i + 1, j
    void swap(int i, int j);

    /// Stops the board from rising
    void freeze(int timeout);

    /// Quick rise the panels
    void quick_rise() {state = FAST_RISING;}

    /// Updates the game board.  Speed is the amount to increase rise_counter by.
    MatchInfo update(int speed);

    int get_rise_counter() const {return rise_counter;}
    int get_rise() const {return rise;}
    int get_timeout() const {return timeout;}
    int get_chain() const {return chain;}
private:
    void init();
    void generate();
    void generate_next();
    MatchInfo update_matches();

    Panel* top(int column) {return &panels[column];}
    const Panel* top(int column) const {return &panels[column];}
    Panel* bottom(int column) {return &panels[(rows - 1) * columns + column];}
    const Panel* bottom(int column) const {return &panels[(rows - 1) * columns + column];}

    bool next_horizontal_error(int j);
    bool next_vertical_error(int j);

    bool vertical(int i, int j);
    bool horizontal(int i, int j);

    std::set<Point> check_horizontal_combo(int i, int j);
    std::set<Point> check_vertical_combo(int i, int j);

    /** Source where panels are generated */
    std::unique_ptr<PanelSource> source;
    /** Speed setttings controlling panel speed */
    PanelSpeedSettings settings;
    /** The panels as a 1d vector */
    std::vector<Panel> panels;
    /** The next set of panels */
    std::vector<Panel> next;
    /** Number of columns */
    int columns;
    /** Number of rows */
    int rows;
    /** Number of moves left */
    int moves = 0;
    /** State of the board */
    State state = RISING;
    /** Game board type */
    Type type = ENDLESS;
    /** Counter to increase rise when this reaches 0x1000 rise is incremented */
    int rise_counter = 0;
    /** Rise amount when this reaches 16 the panels are risen and a new line is generated */
    int rise = 0;
    /** Are we currently stopped */
    bool stopped = false;
    /** Hold long we are stopped */
    int timeout = 0;
    /** Clink counter */
    int clink = 0;
    /** Chain counter */
    int chain = 0;
    /** Lines raised */
    int lines = 0;
};

#endif
