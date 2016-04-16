#include "panel_table.hpp"
#include "random.hpp"
#include <list>
#include <cstdlib>
#include <ctime>
#include <cstdio>

PanelTable::PanelTable(int height, int width, int num_colors) : panels(width * (height + 1)), rows(height), columns(width), colors(num_colors), state(RISING), type(RISES), chain(0), cascade(0)
{
    generate();
}

PanelTable::PanelTable(int height, int width, int num_colors, const Panel::Type* data) : panels(width * (height + 1)), rows(height), columns(width), colors(num_colors), type(RISES), chain(0), cascade(0)
{
    for (int i = 0; i <= rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            panels[i * columns + j].value = *data++;
        }
    }
}

void PanelTable::generate()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            panels[i * columns + j].value = Panel::Type::EMPTY;
        }
    }

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
            set(rows - 1 - i, j, Panel::random(colors));
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            while (vertical(i, j))
                set(i + 1, j, Panel::random(colors));
            while (horizontal(i, j) || (i >= 2 && vertical(i - 2, j + 1)) || (i >= 1 && vertical(i - 1, j + 1)))
                set(i, j + 1, Panel::random(colors));
        }
    }

    generate_next();
}

void PanelTable::generate_next()
{
    for (int j = 0; j < columns; j++)
        set(rows, j, Panel::random(colors));

    for (int j = 0; j < columns; j++)
    {
        while(next_vertical_error(j) || (j >= 2 && next_horizontal_error(j - 2)) || (j >= 1 && next_horizontal_error(j - 1)))
            set(rows, j, Panel::random(colors));
        while (next_horizontal_error(j) && j + 2 < columns)
            set(rows, j + 2, Panel::random(colors));
    }
}

bool PanelTable::next_vertical_error(int j)
{
    if (!matchable(rows - 2, j) || !matchable(rows - 1, j))
        return false;

    Panel::Type k, l, m;
    k = value(rows, j);
    m = value(rows - 1, j);
    l = value(rows - 2, j);

    return k == l && k == m;
}

bool PanelTable::next_horizontal_error(int j)
{
    if (j > columns - 3)
        return false;

    Panel::Type k, l, m;
    m = value(rows, j);
    k = value(rows, j + 1);
    l = value(rows, j + 2);

    return m == k && m == l;
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

void PanelTable::swap(int i, int j)
{
    Panel& left = get(i, j);
    Panel& right = get(i, j + 1);

    if (left.value == right.value || !left.swappable() || !right.swappable())
        return;

    left.swap(right.value, true);
    right.swap(left.value, false);
}

MatchInfo PanelTable::update_matches(void)
{
    MatchInfo match_info;
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

    match_info.combo = remove.size();
    match_info.chain = chain;
    match_info.cascade = cascade;
    match_info.swap_match = !remove.empty();
    match_info.fall_match = false;

    int index = remove.size() - 1;
    for (const auto& pt : remove)
    {
        auto& panel = get(pt.y, pt.x);
        match_info.fall_match |= panel.is_fall_end();
        match_info.swap_match &= !panel.is_fall_end();
        panel.match(index, remove.size());
        index--;
    }

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

MatchInfo PanelTable::update(long time, int max_wait, bool fast_rise)
{
    bool need_update_matches = false;
    bool need_generate_next = false;
    bool in_chain = false;
    bool in_cascade = false;

    if (is_rised() && (type & RISES))
    {
        for (int i = 1; i < rows + 1; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                panels[(i - 1) * columns + j] = panels[i * columns + j];
            }
        }
        need_update_matches = true;
        need_generate_next = true;
    }

    // Iterate in reverse so that falls work correctly.
    for (int y = rows - 1; y >= 0; y--)
    {
        for (int x = 0; x < columns; x++)
        {
            auto& panel = get(y, x);
            if (panel.empty() && panel.is_idle()) continue;

            auto& below = get(y + 1, x);
            panel.update();

            if (panel.is_fall_end())
            {
                Panel& below = get(y + 1, x);
                if (y + 1 < rows && below.empty() && below.is_idle() && !panel.empty())
                {
                    below.value = panel.value;
                    panel.value = Panel::Type::EMPTY;
                    below.fall(true, panel.cascade);
                    panel.state = Panel::State::IDLE;
                }
                else
                    need_update_matches = true;
            }
            else if (panel.is_idle() && !panel.empty() && y < rows - 1)
            {
                if (below.is_falling_process() || below.empty())
                    panel.fall(false, below.is_match_end() || below.cascade);
            }

            if (panel.is_swapped())
                need_update_matches = true;

            in_chain |= panel.is_match_process();
            in_cascade |= panel.cascade;
        }
    }

    if (!in_chain)
    {
        chain = 0;
    }
    if (!(in_chain || in_cascade))
    {
        cascade = 0;
    }

    MatchInfo info;
    if (need_update_matches)
        info = update_matches();
    if (need_generate_next)
        generate_next();

    if (info.fall_match)
        cascade++;
    if (info.swap_match)
        chain++;

    // Board is stopped while matches are being removed.
    if (in_chain)
        return info;

    if (is_rising())
    {
        if (fast_rise) state = FAST_RISING;

        rise += state == FAST_RISING ? (max_wait / 9) : time;

        if ((int) rise >= max_wait)
        {
            rise = 0;
            if (is_danger())
                state = CLOGGED;
            else
                state = RISED;
        }
    }
    else if (is_rised())
    {
        state = RISING;
    }
    else if (is_clogged())
    {
        if (!is_danger())
        {
            state = RISED;
        }
        else
        {
            rise += time;
            if ((int) rise >= max_wait)
                state = GAMEOVER;
        }
    }

    return info;
}
