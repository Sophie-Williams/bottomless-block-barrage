#include "replay_simulation.hpp"

PanelSpeedSettings easy_speed_settings   = {4, 6, 4, 66, 33, 11, FALL_ANIMATION_FRAMES, false};

void copy_trace_to_table(const TraceState& trace, PanelTable& table)
{
    for (unsigned int i = 0; i < table.panels.size(); i++)
        table.panels[i].value = (Panel::Type) (trace.panels[i] & 0xFF);
}

ReplaySimulation::ReplaySimulation(const TraceManager& trace_manager) : traces(trace_manager), last_input(0, 0)
{
    table.create(12, 6, 6, easy_speed_settings);
    copy_trace_to_table(traces.GetInitialState(), table);
}

void ReplaySimulation::Step()
{
    const auto& trace_ptr = traces.GetState(frame);
    const auto& input_ptr = traces.GetInput(frame);
    if (trace_ptr == nullptr || input_ptr == nullptr) return;

    const auto& trace = *trace_ptr;
    const auto& input = *input_ptr;

    if (input.button_a() && !last_input.button_a())
        table.swap(trace.selector_y, trace.selector_x);


    last_input = input;
    table.update(frame, 10000, false);
    frame++;
}

void ReplaySimulation::Run(bool debug)
{
    const TraceState* trace = traces.GetState(frame);
    while (trace)
    {
        Step();
        if (debug) Print();
        trace = traces.GetState(frame);
    }
}

void ReplaySimulation::Print()
{
    const auto& trace_ptr = traces.GetState(frame-1);
    const auto& input_ptr = traces.GetInput(frame-1);
    if (trace_ptr == nullptr || input_ptr == nullptr) return;

    const auto& trace = *trace_ptr;
    const auto& input = *input_ptr;

    printf("frame: %d\ninput: %x\naddress: %04x (%d %d) = %02x\nselector: (%d %d)\n", frame - 1, input.value(), trace.address, trace.y, trace.x, trace.value, trace.selector_y, trace.selector_x);
    for (unsigned int i = 0; i < 13; i++)
    {
        for (unsigned int j = 0; j < 6; j++)
        {
            printf("%08x ", trace.panels[j + i * 6]);
        }
        printf("\t\t");
        for (unsigned int j = 0; j < 6; j++)
        {
            const auto& panel = table.get(i, j);
            printf("%04x%04x ", panel.state, panel.value);
        }

        printf("\n");
    }
    printf("\n\n");
}
