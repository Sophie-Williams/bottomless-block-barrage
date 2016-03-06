#include "panel_table.hpp"
#include "util/random.hpp"
#include <list>
#include <cstdlib>
#include <ctime>
#include <cstdio>


Panel::State& Panel::State::operator=(const Panel::State& other)
{
    state = other.state;
    switch_to = other.switch_to;
    data = other.data;
    return *this;
}

void Panel::State::update()
{
    if (is_swapping())
    {
        data -= 1;
        if (data <= 0) state = State::SWAPPED;
    }
    else if (is_swapped())
    {
        state = State::NORMAL;
    }
    else if (is_pending_fall())
    {
        data -= 1;
        if (data <= 0)
        {
            state = State::FALLING;
            data = FALLING_FRAMES;
        }
    }
    else if (is_falling())
    {
        data -= 1;
        if (data <= 0)
        {
            state = State::END_FALL;
        }
    }
    else if (is_fall_end())
    {
        state = State::NORMAL;
    }
}

Panel& Panel::operator=(const Panel& other)
{
    value = other.value;
    state = other.state;
    return *this;
}

void Panel::update()
{
    /* Handled by panel_table
        if (state.is_fall_end())
            value = Type::EMPTY;*/

    state.update();

   if (state.is_swapped())
        value = state.swap_to();
}

void Panel::swap(Panel::Type swap_to, bool is_left)
{
    is_left ? state.left_swap(swap_to) : state.right_swap(swap_to);
}


PanelTable::PanelTable(int height, int width, int num_colors) : panels(width * (height + 1)), rows(height), columns(width), colors(num_colors)
{
    init();
}

PanelTable::PanelTable(int height, int width, int num_colors, const int* data) : panels(width * (height + 1)), rows(height), columns(width), colors(num_colors)
{
    for (int i = 0; i <= rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            panels[i * columns + j].x = j;
            panels[i * columns + j].y = i;
            panels[i * columns + j].value = (Panel::Type) *data++;
        }
    }
}

void PanelTable::init()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            panels[i * columns + j].x = j;
            panels[i * columns + j].y = i;
            panels[i * columns + j].value = Panel::Type::EMPTY;
        }
    }
    randomize();
}

void PanelTable::randomize()
{
    std::vector<int> values(columns, 0);

    while(1)
    {
        int total = columns * 5;
        for (int i = 0; i < columns - 1; i++)
        {
            int value = randomInt(rows - 3);
            values[i] = value;
            total -= value;
        }
        if (total >= 0 && total <= rows - 3)
        {
            values[columns - 1] = total;
            break;
        }
    }

    for (int j = 0; j < columns; j++)
    {
        int value = values[j];
        for (int i = 0; i < value; i++)
        {
            set(rows - 1 - i, j, (Panel::Type)randomInt(1, colors));
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            while (vertical_error(i, j))
                set(i + 1, j, (Panel::Type)randomInt(1, colors));
            while (horizontal_error(i, j))
                set(i, j + 1, (Panel::Type)randomInt(1, colors));
        }
    }
    setup_next();
}

bool PanelTable::vertical_error(int i, int j)
{
    return vertical(i, j);
}

bool PanelTable::horizontal_error(int i, int j)
{
    return horizontal(i, j) || t_shape(i, j) || cross_shape(i, j);
}

bool PanelTable::vertical(int i, int j)
{
    if (i > rows - 3)
        return false;

    if (empty(i, j) || special(i, j))
        return false;

    if (!matchable(i, j) || !matchable(i + 1, j) || !matchable(i + 2, j))
        return false;

    Panel::Type k, l, m;
    k = value(i, j);
    l = value(i + 1, j);
    m = value(i + 2, j);

    return k == l && k == m;
}

bool PanelTable::horizontal(int i, int j)
{
    if (j > columns - 3)
        return false;

    if (empty(i, j) || special(i, j))
        return false;

    if (!matchable(i, j) || !matchable(i, j + 1) || !matchable(i, j + 2))
        return false;

    Panel::Type k, l, m;
    k = value(i, j);
    l = value(i, j + 1);
    m = value(i, j + 2);

    return k == l && k == m;
}

bool PanelTable::t_shape(int i, int j)
{
    return i >= 2 && vertical(i - 2, j + 1);
}

bool PanelTable::cross_shape(int i, int j)
{
    return i >= 1 && vertical(i - 1, j + 1);
}

void PanelTable::swap(int i, int j)
{
    Panel& left = get(i, j);
    Panel& right = get(i, j + 1);

    if (left.value == right.value && !(left.swappable() && right.swappable()))
        return;
    left.swap(right.value, true);
    right.swap(left.value, false);
}

MatchInfo PanelTable::update_matches(void)
{
    MatchInfo match_info;
    int length;

    do
    {
        std::set<Point> remove = check_for_matches();
        length = remove.size();
        match_info.chain(length);
        for (const auto& pt : remove)
            set(pt.y, pt.x, Panel::Type::EMPTY);
        for (const auto& pt : remove)
            stack_down(pt.y - 1, pt.x);
    }
    while (length != 0);

    return match_info;
}

bool PanelTable::is_danger() const
{
    for (int j = 0; j < columns; j++)
    {
        if (!empty(0, j))
            return true;
    }
    return false;
}

bool PanelTable::is_warning() const
{
    for (int j = 0; j < columns; j++)
    {
        if (!empty(1, j))
            return true;
    }
    return false;
}

std::set<Point> PanelTable::check_for_matches(void)
{
    std::set<Point> remove;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::set<Point> vert;
            std::set<Point> horiz;
            if (horizontal(i, j))
                horiz = check_horizontal_combo(i, j);
            if (vertical(i, j))
                vert = check_vertical_combo(i, j);
            remove.insert(horiz.begin(), horiz.end());
            remove.insert(vert.begin(), vert.end());
        }
    }

    return remove;
}

std::set<Point> PanelTable::check_horizontal_combo(int i, int j)
{
    std::set<Point> remove;
    int moveon = 3;

    remove.emplace(j, i);
    remove.emplace(j + 1, i);
    remove.emplace(j + 2, i);

    for (int k = 3; k < columns - j - 1; k++)
    {
        if (!(j + k < columns && value(i, j) == value(i, j + k) && matchable(i, j + k)))
            break;

        moveon = k + 1;
        remove.emplace(j + k, i);
    }

    for (int m = 0; m < moveon; m++)
    {
        for (int k = -2; k <= 1; k++)
        {
            if (k == 0)
                continue;

            int l = k == -1 ? 1 : k + 1;
            Panel::Type panel = value(i, j);
            if (i + k >= 0 && i + l < rows && panel == value(i + k, j + m) && panel == value(i + l, j + m) && matchable(i + k, j + m) && matchable(i + l, j + m))
            {
                remove.emplace(j + m, i + k);
                remove.emplace(j + m, i + l);
            }
        }
    }

    return remove;
}

std::set<Point> PanelTable::check_vertical_combo(int i, int j)
{
    std::set<Point> remove;
    int moveon = 3;

    remove.emplace(j, i);
    remove.emplace(j, i + 1);
    remove.emplace(j, i + 2);

    for (int k = 3; k < rows - i - 1; k++)
    {
        if (!(i + k < rows && value(i, j) == value(i + k, j) && matchable(i + k, j)))
            break;

        moveon = k + 1;
        remove.emplace(j, i + k);
    }


    for (int m = 0; m < moveon; m++)
    {
        for (int k = -2; k <= 1; k++)
        {
            if (k == 0)
                continue;
            int l = k == -1 ? 1 : k + 1;
            Panel::Type panel = value(i, j);
            if (j + k >= 0 && j + l < columns && panel == value(i + m, j + k) && panel == value(i + m, j + l) && matchable(i + m, j + k) && matchable(i + m, j + l))
            {
                remove.emplace(j + k, i + m);
                remove.emplace(j + l, i + m);
            }
        }
    }

    return remove;
}

bool PanelTable::fall_down(int i, int j, bool already_falling)
{
    if (i < 0 || i >= rows || empty(i, j))
        return false;

    Panel& panel = get(i, j);
    Panel& panel_below = get(i + 1, j);
    const Panel::State& state = panel_below.get_state();

    if (!panel.empty() && (panel_below.empty() || state.is_falling_process()))
    {
        panel.fall(already_falling);
        return true;
    }

    return false;
}

void PanelTable::stack_down(int i, int j)
{
    if (i < 0 || i >= rows - 1)
        return;

    for (; i >= 0; i--)
    {
        Panel& below = get(i + 1, j);
        const Panel::State& state = below.get_state();

        if (!empty(i, j) && (below.empty() || state.is_falling_process()))
            get(i, j).fall(false);
    }
}

void PanelTable::rise()
{
    for (int i = 1; i < rows + 1; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            panels[(i - 1) * columns + j] = panels[i * columns + j];
        }
    }

    update_matches();
    setup_next();
}

void PanelTable::setup_next()
{
    for (int j = 0; j < columns; j++)
        set(rows, j, (Panel::Type)randomInt(1, colors));

    for (int j = 0; j < columns; j++)
    {
        while(next_vertical_error(j) || (j >= 2 && next_horizontal_error(j - 2)) || (j >= 1 && next_horizontal_error(j - 1)))
            set(rows, j, (Panel::Type)randomInt(1, colors));
        while (next_horizontal_error(j) && j + 2 < columns)
            set(rows, j + 2, (Panel::Type)randomInt(1, colors));
    }
}

bool PanelTable::next_vertical_error(int j)
{
    Panel::Type panel = value(rows, j);
    return !empty(rows, j) && panel == value(rows - 2, j) && panel == value(rows - 1, j);
}

bool PanelTable::next_horizontal_error(int j)
{
    if (j > columns - 3)
        return false;

    Panel::Type k, l, m;
    m = value(rows, j);
    k = value(rows, j + 1);
    l = value(rows, j + 2);

    return !empty(rows, j) && m == k && m == l;
}

void PanelTable::State::update(long time, int max_wait, bool danger, bool fast_rise)
{
    if (is_rising())
    {
        if (fast_rise) state = State::FAST_RISING;

        data += state == (State::FAST_RISING) ? (max_wait / 9) : time;

        if ((int)data >= max_wait)
        {
            data = 0;
            if (danger)
                state = State::CLOGGED;
            else
                state = State::RISED;
        }
    }
    else if (is_rised())
    {
        state = State::RISING;
    }
    else if (is_clogged())
    {
        if (!danger)
        {
            state = State::RISED;
        }
        else
        {
            data += time;
            if ((int)data >= max_wait) state = State::GAMEOVER;
        }
    }
}

void PanelTable::update(long time, int max_wait, bool fast_rise)
{
    bool need_update_matches = false;
    if (state.is_rised() && !disable_rise)
        rise();

    // Iterate in reverse so that falls work correctly.
    for (auto panel_it = panels.rbegin(); panel_it != panels.rend(); panel_it++)
    {
        auto& panel = *panel_it;
        Panel::State& state = panel.get_state();
        panel.update();

        if (state.is_swapped())
        {
            if (!fall_down(panel.y, panel.x, false))
                need_update_matches = true;
            if (empty(panel.y, panel.x) && !empty(panel.y - 1, panel.x))
                stack_down(panel.y - 1, panel.x);
        }
        if (state.is_fall_end())
        {
            Panel& below = get(panel.y + 1, panel.x);
            const Panel::State& below_state = below.get_state();
            if (below.empty())
            {
                below.value = panel.value;
                panel.value = Panel::Type::EMPTY;
                if (!fall_down(below.y, below.x, true))
                    need_update_matches = true;
            }
            if (below_state.is_falling_process())
            {
                panel.fall(true);
            }
        }
    }

    if (need_update_matches)
        update_matches();

    state.update(time, max_wait, is_danger(), fast_rise);
}
