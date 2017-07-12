#include "panel_table.hpp"

PanelTable::PanelTable(const Options& opts) : source(opts.source), settings(opts.settings), panels(opts.columns * opts.rows), next(opts.columns), columns(opts.columns), rows(opts.rows), moves(opts.moves),
    type(opts.type)
{
    if (type == ENDLESS)
        state = RISING;
    if (type == MOVES)
        state = PUZZLE;

    init();
    generate();
}

void PanelTable::clear()
{
    for (auto& panel : panels)
        panel.type = Panel::Type::EMPTY;

    for (auto& panel : next)
        panel.type = Panel::Type::EMPTY;
}

void PanelTable::init()
{
    // Handle plumbing things together
    for (auto& panel : next)
    {
        panel.settings = &settings;
        panel.state = Panel::State::BOTTOM;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            int id = j + i * columns;
            auto& panel = panels[id];
            panel.settings = &settings;
            panel.up    = i == 0           ? nullptr  : &panels[id - columns];
            panel.down  = i == rows - 1    ? &next[j] : &panels[id + columns];
            panel.left  = j == 0           ? nullptr  : &panels[id - 1];
            panel.right = j == columns - 1 ? nullptr  : &panels[id + 1];
        }
    }
}

void PanelTable::generate()
{
    clear();

    // Generate initial board.
    std::vector<Panel::Type> values = source->board();
    for (unsigned int i = 0; i < panels.size(); i++)
        panels[i].type = values[i];

    // Correction for 3 in a row/column.
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            int id = i * columns + j;
            while (vertical(i, j))
                panels[id + columns].type = source->panel();
            while (horizontal(i, j) || (i >= 2 && vertical(i - 2, j + 1)) || (i >= 1 && vertical(i - 1, j + 1)))
                panels[id + 1].type = source->panel();
        }
    }

    if (state == RISING)
        generate_next();
}

void PanelTable::generate_next()
{
    const auto& next_panels = source->line();
    for (int i = 0; i < columns; i++)
    {
        next[i].type = next_panels[i];
    }

    for (int j = 0; j < columns; j++)
    {
        while(next_vertical_error(j) || (j >= 2 && next_horizontal_error(j - 2)) || (j >= 1 && next_horizontal_error(j - 1)))
            next[j].type = source->panel();
        while (next_horizontal_error(j) && j + 2 < columns)
            next[j + 1].type = source->panel();
    }
}

bool PanelTable::warning() const
{
    for (int j = 0; j < columns; j++)
    {
        // Row 1
        if (!top(j)->down->empty())
            return true;
    }
    return false;
}

bool PanelTable::danger() const
{
    for (int j = 0; j < columns; j++)
    {
        // row 0
        if (!top(j)->empty())
            return true;
    }
    return false;
}

std::vector<bool> PanelTable::danger_columns() const
{
    std::vector<bool> danger(columns, false);
    for (int j = 0; j < columns; j++)
        danger[j] = !top(j)->empty();
    return danger;
}

void PanelTable::swap(int i, int j)
{
    Panel& left = panels[i * columns + j];

    if (!left.can_swap() || (type == MOVES && moves <= 0))
        return;

    left.swap();

    if (type == MOVES)
        moves -= 1;
}

void PanelTable::timeout(int timeout)
{
    cooloff = timeout;
    stopped = true;
}

MatchInfo PanelTable::update(int speed)
{
    bool need_update_matches = false;
    bool need_generate_next = false;
    bool all_idle = true;
    bool in_clink = false;
    bool in_chain = false;

    if (is_rised())
    {
        for (int j = 0; j < columns; j++)
            top(j)->rise();

        need_update_matches = true;
        need_generate_next = true;
        // Need to do this here if we stop at this exact frame blocks will rise a lot.
        state = RISING;
        lines++;
    }

    // Iterate in reverse so that falls work correctly.
    for (unsigned int i = 0; i < panels.size(); i++)
    {
        auto& panel = panels[panels.size() - 1 - i];
        in_chain |= panel.chain;
        need_update_matches |= panel.update();
        in_clink |= panel.is_match_process();
        all_idle &= panel.is_idle();
    }

    if (!in_clink)
        clink = 0;
    if (!(in_clink || in_chain))
        chain = 0;

    MatchInfo info;
    if (need_update_matches)
        info = update_matches();
    if (need_generate_next)
        generate_next();

    if (info.fall_match)
        chain++;
    if (info.swap_match)
        clink++;

    info.clink = clink <= 1 ? 0 : clink - 1;
    info.chain = chain == 0 ? 0 : chain + 1;

    // Board is stopped while matches are being removed.
    if (in_clink)
        return info;

    if (is_puzzle())
    {
        bool win = true;
        bool idle = true;
        for (const auto& panel : panels)
        {
            win = win && panel.empty();
            idle = idle && panel.is_idle();
        }
        if ((moves == 0 || win) && idle)
            state = win ? WIN : GAMEOVER;
    }
    else if (is_rising() && all_idle)
    {
        if (state == FAST_RISING)
            rise_counter = 0x1000;

        if (rise_counter >= 0x1000)
        {
            rise++;
            rise_counter -= 0x1000;
        }

        rise_counter += speed;

        if (rise >= 16)
        {
            rise = 0;
            if (danger())
                // This state could immediately transition to Game over if !allow_clogged_state
                state = CLOGGED;
            else
                state = RISED;
        }
    }
    /*else if (is_rised())*/ // Handled above.
    else if (is_clogged() && all_idle)
    {
        // For endless mode
        if (type == ENDLESS)
            state = GAMEOVER;

        if (!danger())
        {
            state = RISED;
        }
        else
        {
            if (state == FAST_RISING)
                rise_counter = 0x1000;

            if (rise_counter >= 0x1000)
            {
                rise++;
                rise_counter -= 0x1000;
            }

            rise_counter += speed;

            if (rise >= 16)
            {
                    state = GAMEOVER;
            }
        }
    }

    return info;
}

MatchInfo PanelTable::update_matches()
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
    match_info.swap_match = !remove.empty();
    match_info.fall_match = false;

    int index = remove.size() - 1;
    for (const auto& pt : remove)
    {
        auto& panel = get(pt.y, pt.x);
        match_info.fall_match |= (panel.is_fall_end() && panel.chain);
        match_info.swap_match &= !(panel.is_fall_end() && panel.chain);
        panel.match(index, remove.size() - 1);
        index--;
        match_info.x = pt.x;
        match_info.y = pt.y;
    }

    return match_info;
}

bool PanelTable::next_vertical_error(int j)
{
    if (!matchable(rows - 2, j) || !matchable(rows - 1, j))
        return false;

    Panel::Type k, l, m;
    k = next[j].type;
    m = value(rows - 1, j);
    l = value(rows - 2, j);

    return k == l && k == m;
}

bool PanelTable::next_horizontal_error(int j)
{
    if (j > columns - 3)
        return false;

    Panel::Type k, l, m;
    m = next[j].type;
    k = next[j + 1].type;
    l = next[j + 2].type;

    return m == k && m == l;
}

bool PanelTable::vertical(int i, int j)
{
    if (i > rows - 3)
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

    if (!matchable(i, j) || !matchable(i, j + 1) || !matchable(i, j + 2))
        return false;

    Panel::Type k, l, m;
    k = value(i, j);
    l = value(i, j + 1);
    m = value(i, j + 2);

    return k == l && k == m;
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

