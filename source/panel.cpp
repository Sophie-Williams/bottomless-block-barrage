#include "panel.hpp"

Panel::Type Panel::random(int colors)
{
    return (Panel::Type)randomInt(1, colors);
}

void Panel::update()
{
    if (is_swapping())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            value = swap_to;
            swap_to = Type::EMPTY;
            state = State::SWAPPED;
        }
    }
    else if (is_swapped())
    {
        state = State::IDLE;
    }
    else if (is_pending_fall())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            state = State::FALLING;
            countdown = settings->falling;
        }
    }
    else if (is_falling())
    {
        countdown -= 1;
        if (countdown <= 0)
            state = State::END_FALL;
    }
    else if (is_fall_end())
    {
        // Panel table handles transitioning to FALLING again.
        state = State::IDLE_FELL;
        countdown = settings->idle_fell;
        cascade = false;
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
            state = State::REMOVED;
            countdown = remove_time - match_time;
        }
    }
    else if (is_removed())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            value = Type::EMPTY;
            state = State::END_MATCH;
        }
    }
    else if (is_match_end())
    {
        state = State::IDLE;
        cascade = false;
    }
}

void Panel::swap(Panel::Type swap_panel, bool is_left)
{
    swap_to = swap_panel;
    state = is_left ? State::LEFT_SWAP : State::RIGHT_SWAP;
    countdown = settings->swap;
}

void Panel::fall(bool already_falling, bool is_cascade)
{
    state = already_falling ? State::FALLING : State::PENDING_FALL;
    countdown = already_falling ? settings->falling : settings->pending_fall;
    cascade = is_cascade;
}

void Panel::match(int index, int total)
{
    state = State::PENDING_MATCH;
    match_time = (index + 1) * settings->match;
    remove_time = (total + 1) * settings->match;
    countdown = settings->pending_match;
}
