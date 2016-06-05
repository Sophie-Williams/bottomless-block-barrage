#ifndef PANEL_TABLE_HPP
#define PANEL_TABLE_HPP

#include "panel.hpp"
#include "basic_puzzle.hpp"
#include <list>
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
    MatchInfo() : chain(0), cascade(0), combo(0), swap_match(false), fall_match(false) {}
    bool empty() const {return chain == 0 && cascade == 0 && combo == 0;}
    bool matched() const {return swap_match || fall_match;}
    bool is_timeout() const {return combo > 3 || cascade > 0;}
    std::string str() const;
    /** Chains are rapidly matching 3 or more blocks before they disappear */
    int chain;
    /** Cascades are sequential matches after blocks are removed */
    int cascade;
    /** Combos are matches of more than 3 blocks */
    int combo;
    /** If any panels was in state IDLE then this will be true (part of a chain) */
    bool swap_match;
    /** If any panels was in state END_FALL this will be true (part of a cascade) */
    bool fall_match;
};

class PanelTable
{
public:
    enum Type
    {
        PUZZLE = 0,
        RISES = 1,
        NLINES = 2,
    };
    enum State
    {
        INVALID = 0,
        RISING = 1,
        FAST_RISING = 2,
        RISED = 3,
        STOPPED = 4,
        CLOGGED = 5,
        GAMEOVER = 6,
        WIN_PUZZLE = 7,
    };

    PanelTable(int rows, int columns, int num_colors, const PanelSpeedSettings& settings);
    PanelTable(const BasicPuzzle& puzzle, const PanelSpeedSettings& ssettings);

    bool is_puzzle() const {return type == PUZZLE;}

    bool is_rising() const {return state == RISING || state == FAST_RISING;}
    bool is_rised() const {return state == RISED;}
    bool is_stopped() const {return state == STOPPED;}
    bool is_clogged() const {return state == CLOGGED;}
    bool is_gameover() const {return state == GAMEOVER;}
    bool is_win_puzzle() const {return state == WIN_PUZZLE;}

    bool is_warning() const;
    bool is_danger() const;

    int width() const {return columns;}
    int height() const {return rows;}
    Panel& get(int i, int j) {return panels[i * columns + j];}
    const Panel& get(int i, int j) const {return panels[i * columns + j];}
    void set(int i, int j, Panel::Type value) {panels[i * columns + j].value = value;}
    Panel::Type value(int i, int j) const {return get(i, j).value;}

    bool empty(int i, int j) const {return get(i, j).empty();}
    bool special(int i, int j) const {return get(i, j).special();}
    bool matchable(int i, int j) const {return get(i, j).matchable();}
    bool normal(int i, int j) const {return get(i, j).normal();}
    bool fallable(int i, int j) const {return get(i, j).fallable();}
    bool swappable(int i, int j) const {return get(i, j).swappable();}

    void generate();
    void generate_next();
    void swap(int i, int j);
    MatchInfo update(long speed, int max_wait, bool fast_rise);
    void set_timeout(int timeout);

    MatchInfo update_matches(void);
    std::string str() const;

    friend class TestFall;
    friend class TestStackDown;
    friend class TestUpdateMatches;

    std::vector<Panel> panels;
    const PanelSpeedSettings settings;
    int rows;
    int columns;
    int colors;
    int moves;

    int state;
    /** Game board type */
    int type;
    /** Rise amount */
    int rise;
    /** Hold long we are stopped */
    int cooloff;
    /** Chain link */
    int chain;
    /** Cascade link */
    int cascade;

private:
    bool next_horizontal_error(int j);
    bool next_vertical_error(int j);

    bool vertical(int i, int j);
    bool horizontal(int i, int j);

    std::set<Point> check_horizontal_combo(int i, int j);
    std::set<Point> check_vertical_combo(int i, int j);
};

#endif
