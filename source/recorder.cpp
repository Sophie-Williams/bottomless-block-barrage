#include "recorder.hpp"
#include <ctime>

#define RECORDER_MAJOR_VERSION 0
#define RECORDER_MINOR_VERSION 2

unsigned long osGetTime();

const std::string generate_filename()
{
    char buffer[128];

    time_t val = osGetTime();
    sprintf(buffer, "/bbb-moves/%s", asctime(gmtime(&val)));
    return buffer;
}

void Recorder::settings(int r, int c, PanelTable::Type t, int d, int l)
{
    rows = r;
    columns = c;
    type = t;
    difficulty = d;
    level = l;
}

void Recorder::set_initial(const std::vector<Panel::Type>& board)
{
    initial = board;
}

void Recorder::add_next(const std::vector<Panel::Type>& next_set)
{
    next.insert(next.end(), next_set.begin(), next_set.end());
}

void Recorder::add_input(unsigned int trigger, unsigned int held)
{
    if (!input.empty())
    {
        Input& i = input.back();
        if (i.trigger == trigger && i.held == held)
        {
            i.frames++;
            return;
        }
    }
    input.emplace_back(trigger, held);
}

bool Recorder::save()
{
    const std::string filename = generate_filename();
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (!file.good()) return false;
    bool ret = save(file);
    file.close();
    return ret;
}

bool Recorder::save(std::ostream& file)
{
    const char magic[4] = {'B', 'B', 'B', 0};

    file.write(magic, sizeof(magic));
    file.put(RECORDER_MAJOR_VERSION);
    file.put(RECORDER_MINOR_VERSION);
    file.put(rows);
    file.put(columns);
    file.put(type);
    file.put(difficulty);
    file.put(level);
    // rows * columns size.
    for (const auto& panel : initial)
        file.put(panel);
    unsigned int size = next.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (const auto& panel : next)
        file.put(panel);

    size = input.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (const auto& move : input)
    {
        file.write(reinterpret_cast<const char*>(&move.trigger), sizeof(move.trigger));
        file.write(reinterpret_cast<const char*>(&move.held), sizeof(move.held));
        file.write(reinterpret_cast<const char*>(&move.frames), sizeof(move.frames));
    }

    return true;
}
