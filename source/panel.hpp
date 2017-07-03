#ifndef PANEL_HPP
#define PANEL_HPP

#include "random.hpp"

#define FALL_ANIMATION_FRAMES 11
#define FALL_ANIMATION_DELAY 4

struct PanelSpeedSettings
{
    int swap;
    int pending_fall;
    int falling;
    int pending_match;
    int first_removed;
    int subsequent_removed;
    int idle_fell;
    // If true in danger go to clogged state, otherwise gameover immediately.
    bool allow_clogged_state;
};

class Panel
{
public:
    enum Type
    {
        EMPTY = 0,
        RED = 1,
        GREEN = 2,
        CYAN = 3,
        YELLOW = 4,
        PURPLE = 5,
        BLUE = 6,
        SILVER = 7,
        SPECIAL = 8,
    };

    enum State
    {
        INVALID = 0,
        /** Doing nothing */
        IDLE = 1,
        /** Pending swap states, the difference is the priority when drawing in these states */
        LEFT_SWAP = 2, // swap_to
        RIGHT_SWAP = 3,
        /** Panel has been swapped */
        SWAPPED = 4,
        /** Panel was swapped and about to fall, transitions directly to end fall now */
        PENDING_FALL = 5, // nothing
        /** Falling panel */
        FALLING = 6,
        /** End of falling sequence for this row, panel table will transition this back to falling if it hasn't landed yet */
        END_FALL = 7,
        /** Dummy state for showing a bounce this state is the same as idle */
        IDLE_FELL = 8, // for animation
        /** Panel has been matched showing blink frame */
        PENDING_MATCH = 9, // index_removal, total_removed
        /** Panel has been matched showing panel face graphic */
        MATCHED = 10,
        /** Panel has been removed from board */
        REMOVED = 11,
        /** Dummy state to go back to idle */
        END_MATCH = 12,
    };

    enum FrameGraphic
    {
        NO_DRAW = -1,
        NORMAL = 0,
        SLIGHT_UP = 1,
        UP = 2,
        DOWN = 3,
        PENDING = 4,
        MATCH = 5,
        BLINK = 6,
        LOST = 7,
        GRAPHICS_SIZE = 8,
    };
    bool empty() const {return value == EMPTY;}
    bool normal() const {return !empty() && !special();}
    bool special() const {return value == SPECIAL;}

    bool fallable() const {return is_idle() || is_fall_end();}
    bool swappable() const {return is_idle() || is_falling() || is_fall_end();}
    bool matchable() const {return is_idle() || is_swapped() || is_fall_end();}

    bool is_idle() const {return state == IDLE || state == IDLE_FELL;}
    bool is_fell_idle() const {return state == IDLE_FELL;}
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
    void fall(bool already_falling, bool is_chain = false);
    void match(int index, int total);
    int frame(int def, int danger_panel, bool danger) const;

    static Type random(int colors);

    // Owned by panel_table
    const PanelSpeedSettings* settings;
    Type value = EMPTY;
    unsigned int modifiers = 0;
    // State information
    int state = IDLE;
    // Old Panel to display when swapping.
    Type old_panel = EMPTY;
    int countdown = 0;
    int match_time = 0;
    int remove_time = 0;
    bool chain = false;
};

#endif
