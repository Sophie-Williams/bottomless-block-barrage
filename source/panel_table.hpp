#ifndef PANEL_TABLE_HPP
#define PANEL_TABLE_HPP

#include "panel.hpp"
#include <list>
#include <set>
#include <vector>

struct Point
{
    Point(int j, int i) : x(j), y(i) {}
    bool operator<(const Point& p) const {return (y != p.y) ? y > p.y : x < p.x;}
    bool operator==(const Point& p) const {return x == p.x && y == p.y;}
    int x;
    int y;
};

class MatchInfo
{
public:
    void chain(int combo) {combos_list.push_back(combo);}
    int chains() const {return combos_list.size();}
    const std::list<int>& combos() const {return combos_list;}
private:
    std::list<int> combos_list;
};

class PanelTable
{
public:
    enum Type
    {
        RISES = 1,
        NLINES = 2,
    };
    enum State
    {
        INVALID = 0,
        RISING = 1,
        FAST_RISING = 2,
        RISED = 3,
        STOPPING = 4,
        STOPPED = 5,
        CLOGGED = 6,
        GAMEOVER = 7,
    };

    PanelTable(int rows, int columns, int num_colors);
    PanelTable(int rows, int columns, int num_colors, const Panel::Type* data);

    bool is_rising() const {return state == RISING || state == FAST_RISING;}
    bool is_rised() const {return state == RISED;}
    bool is_stopping() const {return state == STOPPING;}
    bool is_stopped() const {return state == STOPPED;}
    bool is_clogged() const {return state == CLOGGED;}
    bool is_gameover() const {return state == GAMEOVER;}

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
    void update(long speed, int max_wait, bool fast_rise);

    MatchInfo update_matches(void);

    friend class TestFall;
    friend class TestStackDown;
    friend class TestUpdateMatches;

    std::vector<Panel> panels;
    int rows;
    int columns;
    int colors;
    int state;
    int type;
    int rise;
    int cooloff;

private:
    std::set<Point> check_for_matches(void);

    bool next_horizontal_error(int j);
    bool next_vertical_error(int j);

    bool vertical(int i, int j);
    bool horizontal(int i, int j);

    std::set<Point> check_horizontal_combo(int i, int j);
    std::set<Point> check_vertical_combo(int i, int j);

};

#endif
