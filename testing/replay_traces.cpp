#include "trace_state.hpp"
#include "panel_table.hpp"
#include <string>

PanelSpeedSettings easy_speed_settings   = {4, 6, 4, 66, 33, 11, FALL_ANIMATION_FRAMES, false};

void copy_trace_to_table(const TraceState& trace, PanelTable& table)
{
    for (unsigned int i = 0; i < table.panels.size(); i++)
        table.panels[i].value = (Panel::Type) (trace.panels[i] & 0xFF);
}

class ReplaySimulation
{
public:
    ReplaySimulation(const TraceManager& trace_manager) : traces(trace_manager), last_input(0, 0)
    {
        table.create(12, 6, 6, easy_speed_settings);
        copy_trace_to_table(traces.GetInitialState(), table);
    }

    void Step()
    {
        const auto& trace_ptr = traces.GetState(frame);
        const auto& input_ptr = traces.GetInput(frame);
        if (trace_ptr == nullptr || input_ptr == nullptr) return;

        const auto& trace = *trace_ptr;
        const auto& input = *input_ptr;

        if (input.button_a() && !last_input.button_a())
            table.swap(trace.selector_y, trace.selector_x);

        printf("frame: %d\ninput: %x\naddress: %04x (%d %d) = %02x\nselector: (%d %d)\n", frame, input.value(), trace.address, trace.y, trace.x, trace.value, trace.selector_y, trace.selector_x);
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

        last_input = input;
        table.update(frame, 10000, false);
        frame++;
    }

    void Run()
    {
        const TraceState* trace = traces.GetState(frame);
        while (trace)
        {
            Step();
            trace = traces.GetState(frame);
        }
    }
private:
    PanelTable table;
    TraceManager traces;
    TraceInput last_input;
    uint32_t frame = 0;
};

int main(int argc, char** argv)
{
    std::string trace = argv[1];

    TraceManager manager = read_trace_file(trace);
    ReplaySimulation simulation(manager);

    simulation.Run();

}

/*
    for (const auto& trace : manager.GetTraces())
    {
        printf("frame: %d\ninput: %x\naddress: %04x (%d %d) = %02x\nselector: (%d %d)\n", trace.frame, trace.input, trace.address, trace.y, trace.x, trace.value, trace.selector_y, trace.selector_x);
        for (unsigned int i = 0; i < 13; i++)
        {
            for (unsigned int j = 0; j < 6; j++)
            {
                printf("%08x ", trace.panels[j + i * 6]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
*/
