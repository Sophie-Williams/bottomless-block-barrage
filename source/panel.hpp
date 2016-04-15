#ifndef PANEL_HPP
#define PANEL_HPP

#include "random.hpp"

#define SWAP_FRAMES /*4*/60
#define PENDING_FALL_FRAMES /*4*/60
#define FALLING_FRAMES /*2*/60
#define PENDING_MATCH_FRAMES 60
#define MATCH_FRAMES 60

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

    enum State
    {
        INVALID = 0,
        IDLE = 1,
        LEFT_SWAP = 2, // swap_to
        RIGHT_SWAP = 3,
        SWAPPED = 4,
        PENDING_FALL = 5, // nothing
        FALLING = 6,
        END_FALL = 7,
        PENDING_MATCH = 8, // index_removal, total_removed
        MATCHED = 9,
        REMOVED = 10,
        END_MATCH = 11,
    };

    bool empty() const {return value == EMPTY;}
    bool normal() const {return !empty() && !special();}
    bool special() const {return value == SPECIAL;}

    bool fallable() const {return state == END_FALL || state == IDLE;}
    bool swappable() const {return state == FALLING || state == END_FALL || state == IDLE;}
    bool matchable() const {return state == IDLE || state == SWAPPED || state == END_FALL;}

    bool is_idle() const {return state == IDLE;}
    bool is_swapping() const {return is_left_swap() || is_right_swap();}
    bool is_left_swap() const {return state == LEFT_SWAP;}
    bool is_right_swap() const {return state == RIGHT_SWAP;}
    bool is_swapped() const {return state == SWAPPED;}
    bool is_pending_fall() const {return state == PENDING_FALL;}
    bool is_falling() const {return state == FALLING;}
    bool is_fall_end() const {return state == END_FALL;}
    bool is_falling_process() const {return is_pending_fall() || is_falling() || is_fall_end();}
    bool is_pending_match() const {return state == PENDING_MATCH;}
    bool is_matched() const {return state == MATCHED;}
    bool is_removed() const {return state == REMOVED;}
    bool is_match_end() const {return state == END_MATCH;}
    bool is_match_process() const {return is_pending_match() || is_matched() || is_removed() || is_match_end();}

    void update();
    void swap(Type swap_to, bool is_left);
    void fall(bool already_falling);
    void match(int index, int total);

    static Type random(int colors);

    Type value = EMPTY;
    unsigned int modifiers = 0;
    // State information
    int state = IDLE;
    Type swap_to = EMPTY;
    int countdown = 0;
    int match_time = 0;
    int remove_time = 0;
};

#endif
