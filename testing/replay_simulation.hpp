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
private:
    PanelTable table;
    TraceManager traces;
    TraceInput last_input;
    uint32_t frame = 0;
};

#endif
