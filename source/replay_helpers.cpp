#include "replay_helpers.hpp"
#include <cstring>
#include "recorder.hpp"

u32 ReplayInputDataSource::trigger() const
{
    if (index >= data.size()) return 0;
    return data[index].trigger;
}

u32 ReplayInputDataSource::held() const
{
    if (index >= data.size()) return 0;
    return data[index].held;
}

void ReplayInputDataSource::update()
{
    if (index >= data.size()) return;
    frame++;
    if (frame - start > data[index].frames)
    {
        start += data[index].frames;
        index++;
    }
}

bool load_replay(const std::string& filename, ReplayInfo& info)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    return load_replay(file, info);
}

bool load_replay(std::istream& file, ReplayInfo& info)
{
    char magic[4];

    file.read(magic, 4);
    if (memcmp(magic, "BBB", 4) != 0)
        return false;

    char major = file.get();
    char minor = file.get();

    if (major != RECORDER_MAJOR_VERSION)
        return false;
    if (minor != RECORDER_MINOR_VERSION)
        return false;

    info.rows = file.get();
    info.columns = file.get();
    info.type = file.get();
    info.difficulty = file.get();
    info.level = file.get();

    std::vector<Panel::Type> initial;
    unsigned int size = info.rows * info.columns;
    initial.reserve(size);
    for (unsigned int i = 0; i < size; i++)
        initial.push_back(static_cast<Panel::Type>(file.get()));

    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<Panel::Type> next;
    next.reserve(size);
    for (unsigned int i = 0; i < size; i++)
        next.push_back(static_cast<Panel::Type>(file.get()));

    info.source.reset(new ReplayPanelSource(info.rows, info.columns, initial, next));

    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<ReplayInputItem> items;
    for (unsigned int i = 0; i < size; i++)
    {
        ReplayInputItem move;
        file.read(reinterpret_cast<char*>(&move), sizeof(move));
        items.emplace_back(move);
    }

    info.input.reset(new ReplayInputDataSource(items));

    return true;
}
