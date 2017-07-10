#include "panel.hpp"


bool Panel::can_match() const
{
    return (is_idle() || is_swapped() || is_fall_end()) && normal();
}

bool Panel::can_swap() const
{
    if (right == nullptr)
        return false;

    // Must be in these states.
    bool left_state = is_idle() || is_falling() || is_fall_end() || is_swapped();
    bool right_state = right->is_idle() || right->is_falling() || right->is_fall_end() || right->is_swapped();

    if (!left_state || !right_state)
        return false;

    // Next determination if I'm not empty and right is empty then right up must not be in a falling state.
    if (!empty() && right->empty() && right->up != nullptr && right->up->is_falling_process())
        return false;

    // Vice versa
    if (empty() && !right->empty() && up != nullptr && up->is_falling_process())
        return false;

    return true;
}

void Panel::swap()
{
    if (right == nullptr) return;

    // Perform the swap immediately.
    Type temp = type;
    type = right->type;
    right->type = temp;

    old = right->type;
    right->old = type;

    state = State::LEFT_SWAP;
    right->state = State::RIGHT_SWAP;

    countdown = right->countdown = settings->swap;
}

int Panel::match(int index, int total)
{
    state = State::PENDING_MATCH;
    match_time = settings->first_removed + index * settings->subsequent_removed;
    remove_time = settings->first_removed + total * settings->subsequent_removed;
    countdown = settings->pending_match;
    return 0;
}

void Panel::rise()
{
    if (down == nullptr)
    {
        type = EMPTY;
        return;
    }
    type = down->type;
    down->rise();
}


bool Panel::update()
{
    if (is_swapping())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            old = Type::EMPTY;
            state = State::SWAPPED;
        }
    }
    else if (is_swapped())
    {
        state = State::IDLE;
    }
    else if (is_falling() || is_pending_fall())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            // If below panel is not blocking and non empty panel
            if (!down->is_bottom() && down->empty() && down->is_idle() && !empty())
            {
                down->type = type;
                down->state = FALLING;
                down->countdown = settings->falling;
                down->chain = chain;

                type = EMPTY;
                state = IDLE;
                chain = false;
            }
            // Slip case: Below panel is in fall process
            else if (!down->is_bottom() && !down->empty() && down->is_falling_process() && !empty())
            {
                state = down->state;
                countdown = down->countdown;
            }
            else
            {
                state = State::END_FALL;
            }
        }
    }
    else if (is_fall_end())
    {
        state = State::IDLE_FELL;
        countdown = settings->idle_fell;
        chain = false;
    }
    else if (is_fell_idle())
    {
        countdown -= 1;
        if (countdown <= 0)
            state = State::IDLE;
    }
    else if (is_pending_match())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            state = State::MATCHED;
            countdown = match_time;
        }
    }
    else if (is_matched())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            countdown = remove_time - match_time;
            // last panel in removed will immediately transition to end match since remove_time == match_time.
            if (countdown == 0)
                type = Type::EMPTY;
            state = countdown == 0 ? State::END_MATCH : State::REMOVED;

        }
    }
    else if (is_removed())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            type = Type::EMPTY;
            state = State::END_MATCH;
        }
    }
    else if (is_match_end())
    {
        state = State::IDLE;
        chain = false;
    }

    // For transitioning to falling
    // Need to do this if it is swapped to prevent this scenario
    // PP P
    // BB B
    // Swap the P over the empty space and it is matched.
    if ((is_idle() || is_swapped()) && !empty() && !down->is_bottom())
    {
        // Can fall if the panel below you is falling or the panel below you is empty and (idle | swapped | match_end)
        if (down->is_falling_process() || (down->empty() && (down->is_idle() || down->is_swapped() || down->is_match_end())))
        {
            state = PENDING_FALL;
            countdown = settings->pending_fall + settings->falling;
            chain = down->is_match_end() || down->chain;
        }
    }

    return is_swapped() || is_fall_end();
}
