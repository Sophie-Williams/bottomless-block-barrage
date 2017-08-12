#ifndef PANEL_HPP
#define PANEL_HPP

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
        /** Panel was swapped and about to fall, transitions to falling after 1 frame */
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
        /** State for next set of panels, can't be match swap or fall */
        BOTTOM = 13,
    };

    bool empty() const {return type == EMPTY;}
    bool special() const {return type == SPECIAL;}
    bool normal() const {return type != EMPTY && type != SPECIAL;}

    Type get_value() const {return type;}
    void set_value(Type value) {type = value;}
    State get_state() const {return state;}
    bool get_chain() const {return chain;}
    int get_countdown() const {return countdown;}

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
    bool is_bottom() const {return state == BOTTOM;}

    /// Can the panel be matched
    bool can_match() const;
    /// Can the panel be swapped.
    bool can_swap() const;

    /// Start matching panels returns number of panels matched
    int match(int index, int total, int types_matched, bool chain = false);
    /// Swaps panel with the one to its right.
    void swap();
    /// Rises the panel up one
    void rise();

    /// Updates the panel, true is returned to trigger a find matches
    bool update();

private:
    /// Mesh panels owned by PanelTable
    Panel* up = nullptr;
    Panel* down = nullptr;
    Panel* left = nullptr;
    Panel* right = nullptr;
    // Owned by panel_table
    const PanelSpeedSettings* settings = nullptr;

    State state = IDLE;
    Type type = EMPTY;
    /// Old type when swapping
    Type old = EMPTY;
    bool chain = false;
    int match_time = 0;
    int remove_time = 0;
    int countdown = 0;
    // Locked from swapping.
    bool locked = false;

    friend class PanelTable;
};

#endif
