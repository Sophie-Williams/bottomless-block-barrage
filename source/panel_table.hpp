#ifndef PANEL_TABLE_HPP
#define PANEL_TABLE_HPP

#include <list>
#include <set>
#include <vector>

#define SWAP_FRAMES /*4*/30
#define PENDING_FALL_FRAMES /*4*/30
#define FALLING_FRAMES /*2*/30

struct Point
{
    Point(int j, int i) : x(j), y(i) {}
    bool operator<(const Point& p) const
    {
        if (y != p.y)
            return y > p.y;
        else
            return x < p.x;
    }
    bool operator==(const Point& p) const {return x == p.x && y == p.y;}
    int x;
    int y;
};

class Panel
{
public:
    enum Type
    {
        EMPTY = 0,
        GREEN = 1,
        PURPLE = 2,
        RED = 3,
        YELLOW = 4,
        CYAN = 5,
        BLUE = 6,
        SILVER = 7,
        SPECIAL = 8,
    };
    class State
    {
    public:
        enum States
        {
            INVALID = 0,
            NORMAL = 1,
            LEFT_SWAP = 2,
            RIGHT_SWAP = 3,
            SWAPPED = 4,
            PENDING_FALL = 5,
            FALLING = 6,
            END_FALL = 7,
            PENDING_MATCH = 8,
            MATCHED = 9,
            REMOVAL = 10,
        };
        State() : state(States::NORMAL), switch_to(Type::EMPTY), data(0) {}
        State& operator=(const State& other);
        void update();
        int state_no() const {return (int) state;}
        int value() const {return data;}
        Type swap_to() const {return switch_to;}
        bool is_normal() const {return state == States::NORMAL;}
        bool is_swapping() const {return state == States::LEFT_SWAP || state == States::RIGHT_SWAP;}
        bool is_swapped() const {return state == States::SWAPPED;}
        bool is_left_swap() const {return state == States::LEFT_SWAP;}
        bool is_right_swap() const {return state == States::RIGHT_SWAP;}
        bool is_pending_fall() const {return state == States::PENDING_FALL;}
        bool is_falling() const {return state == States::FALLING;}
        bool is_fall_end() const {return state == States::END_FALL;}
        bool is_falling_process() const {return is_pending_fall() || is_falling() || is_fall_end();}
        bool is_matchable() const {return is_normal() || is_swapped() || is_fall_end();}
        bool is_swappable() const {return is_normal() || is_falling_process();}
        bool is_fallable() const {return is_normal() || is_swapped() || is_fall_end();}

        void left_swap(Type type) {state = States::LEFT_SWAP; switch_to = type; data = SWAP_FRAMES;}
        void right_swap(Type type) {state = States::RIGHT_SWAP; switch_to = type; data = SWAP_FRAMES;}
        void fall(bool already_falling)
        {
            if (!is_fallable()) return;
            if (already_falling)
            {
                state = States::FALLING;
                data = FALLING_FRAMES;
            }
            else
            {
                state = States::PENDING_FALL;
                data = PENDING_FALL_FRAMES;
            }
        }
        friend class TestFall;
        friend class TestStackDown;
        friend class TestUpdateMatches;
    private:
        States state;
        Type switch_to;
        int data;
    };
    Panel& operator=(const Panel& other);
    bool empty() const {return value == Type::EMPTY;}
    bool special() const {return value == Type::SPECIAL;}
    bool matchable() const {return state.is_matchable();}
    bool swappable() const {return state.is_swappable();}
    void update();
    void swap(Type swap_to, bool is_left);
    void fall(bool already_falling) {state.fall(already_falling);}
    const State& get_state() const {return state;}
    State& get_state() {return state;}
    int x;
    int y;
    Type value;
    State state;
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
    class State
    {
    public:
        enum States
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
        State() : state(RISING), data(0) {}
        bool is_rising() const {return state == States::RISING || state == States::FAST_RISING;}
        bool is_rised() const {return state == States::RISED;}
        bool is_stopping() const {return state == States::STOPPING;}
        bool is_stopped() const {return state == States::STOPPED;}
        bool is_clogged() const {return state == States::CLOGGED;}
        bool is_gameover() const {return state == States::GAMEOVER;}
        int value() const {return data;}
        void update(long speed, int max_wait, bool danger, bool fast_rise);

    private:
        States state;
        unsigned long data;
    };
    PanelTable(int rows, int columns, int num_colors);
    PanelTable(int rows, int columns, int num_colors, const int* data);
    int width() const {return columns;}
    int height() const {return rows;}
    Panel& get(int i, int j) {return panels[i * columns + j];}
    const Panel& get(int i, int j) const {return panels[i * columns + j];}
    void set(int i, int j, Panel::Type value) {panels[i * columns + j].value = value;}
    Panel::Type value(int i, int j) const {return get(i, j).value;}
    bool empty(int i, int j) const {return get(i, j).empty();}
    bool special(int i, int j) const {return get(i, j).special();}
    bool matchable(int i, int j) const {return get(i, j).matchable();}
    const State& get_state() const {return state;}

    void init();
    void swap(int i, int j);
    bool is_warning() const;
    bool is_danger() const;
    MatchInfo update_matches(void);
    void update(long speed, int max_wait, bool fast_rise);

    friend class TestFall;
    friend class TestStackDown;
    friend class TestUpdateMatches;
private:
    std::set<Point> check_for_matches(void);

    void randomize();
    bool vertical_error(int i, int j);
    bool horizontal_error(int i, int j);
    bool next_horizontal_error(int j);
    bool next_vertical_error(int j);

    bool vertical(int i, int j);
    bool horizontal(int i, int j);
    bool t_shape(int i, int j);
    bool cross_shape(int i, int j);

    void stack_down(int i, int j);
    bool fall_down(int i, int j, bool already_falling);

    void setup_next();
    void rise();

    std::set<Point> check_horizontal_combo(int i, int j);
    std::set<Point> check_vertical_combo(int i, int j);

    std::vector<Panel> panels;
    State state;
    int rows;
    int columns;
    int colors;
    bool disable_rise;
};

#endif
