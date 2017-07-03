#include "panel_table.hpp"
#include "random.hpp"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <list>
#include <sstream>

#define VERSION_MAJOR 1
#define VERSION_MINOR 1

#define MAX_PUZZLE_ROWS 12
#define MAX_PUZZLE_COLUMNS 6
#define MAX_PUZZLE_MOVES 1000000

void load_version_1_0(const BasicPuzzle& puzzle, PanelTable& table)
{
    // Version 1.0 had 11x6 board for puzzle mode with 6 extra padding bytes which contain junk.
    table.type = puzzle.type;
    table.rows = 12;
    table.columns = puzzle.columns;
    table.moves = puzzle.moves;

    table.panels.resize(table.columns * table.rows);
    for (unsigned int i = 0; i < table.panels.size(); i++)
    {
        table.panels[i].value = (int)i < table.columns ? Panel::EMPTY : (Panel::Type) puzzle.panels[i - table.columns];
        table.panels[i].settings = &table.settings;
    }
}

PanelTable::PanelTable(int height, int width, int num_colors, const PanelSpeedSettings& ssettings) :
     panels(width * (height + 1)), settings(ssettings), rows(height), columns(width), colors(num_colors), state(RISING),
     type(RISES), clink(0), chain(0), lines(0)
{
    generate();
}

PanelTable::PanelTable(const std::string& filename, const PanelSpeedSettings& ssettings)
{
    create(filename, ssettings);
}

void PanelTable::create(int height, int width, int num_colors, const PanelSpeedSettings& ssettings)
{
    panels.resize(width * (height + 1));
    settings = ssettings;
    rows = height;
    columns = width;
    colors = num_colors;

    generate();
}

void PanelTable::create(const std::string& filename, const PanelSpeedSettings& ssettings)
{
    settings = ssettings;
    rise = 0;
    cooloff = 0;
    clink = 0;
    chain = 0;
    lines = 0;
    state = PUZZLE;

    BasicPuzzle puzzle;
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file)
        return;
    fread(&puzzle, sizeof(BasicPuzzle), 1, file);
    fclose(file);

    char* magic = puzzle.magic;
    if (!(magic[0] == 'B' && magic[1] == 'B' && magic[2] == 'B' && magic[3] == 0))
        return;

    char* version = puzzle.version;
    if (version[0] > VERSION_MAJOR || (version[0] == VERSION_MAJOR && version[1] > VERSION_MINOR))
        return;

    if (puzzle.type != PUZZLE)
        return;

    if (puzzle.rows > MAX_PUZZLE_ROWS || puzzle.columns > MAX_PUZZLE_COLUMNS || puzzle.starting > MAX_PUZZLE_ROWS ||
        puzzle.moves > MAX_PUZZLE_MOVES)
        return;

    if (version[0] == 1 && version[1] == 0)
    {
        load_version_1_0(puzzle, *this);
        return;
    }

    type = puzzle.type;
    rows = puzzle.rows;
    columns = puzzle.columns;
    moves = puzzle.moves;

    panels.resize(columns * rows);
    for (unsigned int i = 0; i < panels.size(); i++)
    {
        panels[i].value = (Panel::Type) puzzle.panels[i];
        panels[i].settings = &settings;
    }
}

void PanelTable::generate()
{
    state = RISING;
    type = RISES;
    rise = 0;
    cooloff = 0;
    clink = 0;
    chain = 0;
    lines = 0;

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

    for (unsigned int i = 0; i < panels.size(); i++)
    {
        panels[i].settings = &settings;
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


bool PanelTable::can_swap(int i, int j) const
{
    const Panel& left = get(i, j);
    const Panel& right = get(i, j + 1);

    return !(left.value == right.value || !left.swappable() || !right.swappable() || (is_puzzle() && moves <= 0));
}

void PanelTable::swap(int i, int j)
{
    Panel& left = get(i, j);
    Panel& right = get(i, j + 1);

    if (left.value == right.value || !left.swappable() || !right.swappable() || (is_puzzle() && moves <= 0))
        return;

    Panel::Type l = left.value;
    Panel::Type r = right.value;

    left.swap(r, true);
    right.swap(l, false);

    moves -= 1;
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
    match_info.swap_match = !remove.empty();
    match_info.fall_match = false;

    int index = remove.size() - 1;
    for (const auto& pt : remove)
    {
        auto& panel = get(pt.y, pt.x);
        match_info.fall_match |= (panel.is_fall_end() && panel.chain);
        match_info.swap_match &= !(panel.is_fall_end() && panel.chain);
        panel.match(index, remove.size());
        index--;
        match_info.x = pt.x;
        match_info.y = pt.y;
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

std::vector<bool> PanelTable::is_danger_columns() const
{
    std::vector<bool> danger;
    for (int j = 0; j < columns; j++)
        danger.push_back(!empty(0, j));
    return danger;
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


bool PanelTable::all_idle() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (!get(i, j).is_idle())
                return false;
        }
    }
    return true;
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
    bool in_clink = false;
    bool in_chain = false;

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
        // Need to do this here if we stop at this exact frame blocks will rise a lot.
        state = RISING;
        lines++;
    }

    // Iterate in reverse so that falls work correctly.
    for (int y = rows - 1; y >= 0; y--)
    {
        for (int x = 0; x < columns; x++)
        {
            auto& panel = get(y, x);
            bool panel_in_chain = panel.chain;
            //if (panel.empty() && panel.is_idle()) continue;

            auto& below = get(y + 1, x);
            panel.update();

            if (panel.is_fall_end())
            {
                Panel& below = get(y + 1, x);
                // If below panel is not blocking and non empty panel
                if (y + 1 < rows && below.empty() && below.is_idle() && !panel.empty())
                {
                    below.value = panel.value;
                    panel.value = Panel::Type::EMPTY;
                    below.fall(true, panel.chain);
                    panel.state = Panel::State::IDLE;
                    panel.chain = false;
                }
                // Slip case: Below panel is in fall process
                else if (y + 1 < rows && !below.empty() && below.is_falling_process() && !panel.empty())
                {
                    panel.state = below.state;
                    panel.countdown = below.countdown;
                }
                else
                    need_update_matches = true;
            }
            // Need to do this if it is swapped to prevent this scenario
            // PP P
            // BB B
            // Swap the P over the empty space and it is matched.
            else if ((panel.is_idle() || panel.is_swapped()) && !panel.empty() && y < rows - 1)
            {
                if (below.is_falling_process() || below.empty())
                    panel.fall(panel.is_idle(), below.is_match_end() || below.chain);
            }

            if (panel.is_swapped())
                need_update_matches = true;

            in_clink |= panel.is_match_process();
            in_chain |= panel_in_chain;
        }
    }

    if (!in_clink)
    {
        clink = 0;
    }
    if (!(in_clink || in_chain))
    {
        chain = 0;
    }

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

    if (stopped && !is_puzzle())
    {
        if (fast_rise)
        {
            stopped = false;
            cooloff = 0;
        }

        cooloff -= time;
        if (cooloff <= 0)
        {
            stopped = false;
            cooloff = 0;
        }
    }
    else if (is_puzzle() && !(is_gameover() || is_win_puzzle()))
    {
        bool win = true;
        bool idle = true;
        for (unsigned int i = 0; i < panels.size(); i++)
        {
            win = win && panels[i].value == Panel::Type::EMPTY;
            idle = idle && panels[i].state == Panel::State::IDLE;
        }
        if (moves == 0 && idle)
            state = win ? WIN_PUZZLE : GAMEOVER;
    }
    else if (is_rising())
    {
        if (fast_rise) state = FAST_RISING;

        rise += state == FAST_RISING ? (max_wait / 9) : time;

        if ((int) rise >= max_wait)
        {
            rise = 0;
            if (is_danger())
                // This state could immediately transition to Game over if !allow_clogged_state
                state = CLOGGED;
            else
                state = RISED;
        }
    }
    /*else if (is_rised())*/ // Handled above.
    else if (is_clogged())
    {
        // For endless mode
        if (!settings.allow_clogged_state && all_idle())
        {
            state = GAMEOVER;
        }

        if (!is_danger())
        {
            state = RISED;
        }
        else
        {
            rise += time;
            if (((int) rise >= max_wait || fast_rise) && all_idle())
                state = GAMEOVER;
        }
    }

    return info;
}

void PanelTable::set_timeout(int timeout)
{
    cooloff = timeout;
    stopped = true;
}

std::string PanelTable::str() const
{
    std::stringstream oss;
    for (unsigned int i = 0; i < panels.size(); i++)
    {
        oss << (int)panels[i].value << "(" << panels[i].state << "|" << panels[i].chain << ") ";
        if (i % columns == (unsigned int)columns - 1) oss << "\n";
    }
    oss << "\n";
    return oss.str();
}

std::string MatchInfo::str() const
{
    std::stringstream oss;
    oss << "Combo: " << combo << "\n"
        << "C-Link: " << clink << "\n"
        << "Chain: " << chain << "\n"
        << "Swapped? " << swap_match << " Fall? " << fall_match << "\n";
    return oss.str();
}
