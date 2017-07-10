#ifndef REPLAY_SIMULATION_HPP
#define REPLAY_SIMULATION_HPP

#include <cassert>
#include <cstdint>
#include <memory>
#include "panel_source.hpp"
#include "panel_table.hpp"
#include "trace_state.hpp"

class ReplayPanelSource : public PanelSource
{
public:
    ReplayPanelSource(const TraceState& initial, const std::vector<Panel::Type>& next);
    ~ReplayPanelSource() override {}
    std::vector<Panel::Type> board() override {return table;}
    Panel::Type panel() override {return Panel::Type::EMPTY;}
    std::vector<Panel::Type> line() override;
    void reset() {index = 0;}
private:
    std::vector<Panel::Type> table;
    std::vector<Panel::Type> next;
    int index;
};

class ReplaySimulation
{
public:
    ReplaySimulation(const TraceManager& trace_manager, const PanelSpeedSettings& settings);
    void Step();
    void Run(bool debug = false);
    void Print();
    const TraceState* GetTraceState() const {return traces.GetState(frame);}
    const TraceInput* GetInput() const {return traces.GetInput(frame);}
    const PanelTable& GetPanelTable() const {return *table;}
    bool Finished() const {return frame > traces.GetFinalFrame();}
    uint32_t GetFrame() const {return frame;}

private:
    std::unique_ptr<PanelTable> table;
    TraceManager traces;
    TraceInput last_input;
    uint32_t frame = 0;
    bool table_has_risen = false;
};

#endif
