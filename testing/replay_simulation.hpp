#ifndef REPLAY_SIMULATION_HPP
#define REPLAY_SIMULATION_HPP

#include <cstdint>

#include "panel_table.hpp"
#include "trace_state.hpp"

class ReplaySimulation
{
public:
    ReplaySimulation(const TraceManager& trace_manager);
    void Step();
    void Run(bool debug = false);
    void Print();
    const TraceState* GetTraceState() const {return traces.GetState(frame);}
    const TraceInput* GetInput() const {return traces.GetInput(frame);}
    const PanelTable& GetPanelTable() const {return table;}
    bool Finished() const {return frame > traces.GetFinalFrame();}
    uint32_t GetFrame() const {return frame;}

private:
    PanelTable table;
    TraceManager traces;
    TraceInput last_input;
    uint32_t frame = 0;
    bool table_has_risen = false;
};

#endif
