#ifndef REPLAY_HELPERS_HPP
#define REPLAY_HELPERS_HPP

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include <util/input_data_source_interface.hpp>

#include "panel_source.hpp"

struct ReplayInputItem
{
    u32 trigger;
    u32 held;
    u32 frames;
};

class ReplayPanelSource : public PanelSource
{
public:
    ReplayPanelSource(int rows, int columns, const std::vector<Panel::Type>& start, const std::vector<Panel::Type> next_set) :
        PanelSource(rows, columns), initial(start), next(next_set), index(0) {}
    ~ReplayPanelSource() {}
    std::vector<Panel::Type> board() override {return initial;}
    Panel::Type panel() override
    {
        if (index >= next.size()) return Panel::EMPTY;
        return next[index++];
    }
private:
    std::vector<Panel::Type> initial;
    std::vector<Panel::Type> next;
    unsigned int index;
};

class ReplayInputDataSource : public InputDataSourceInterface
{
public:
    ReplayInputDataSource(const std::vector<ReplayInputItem>& items) : data(items), index(0) {}
    ~ReplayInputDataSource() {}
    u32 trigger() const override;
    u32 held() const override;
    void update() override;
private:
    std::vector<ReplayInputItem> data;
    unsigned int index;
    unsigned int start;
    unsigned int frame;

};

struct ReplayInfo
{
    std::unique_ptr<ReplayPanelSource> source;
    std::unique_ptr<ReplayInputDataSource> input;
    char rows;
    char columns;
    char type;
    char difficulty;
    char level;
};

bool load_replay(const std::string& filename, ReplayInfo& info);
bool load_replay(std::istream& file, ReplayInfo& info);

#endif
