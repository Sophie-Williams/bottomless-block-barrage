#include "trace_state.hpp"
#include "replay_simulation.hpp"
#include <string>

int main(int argc, char** argv)
{
    std::string trace = argv[1];

    TraceManager manager = read_trace_file(trace);
    ReplaySimulation simulation(manager);

    simulation.Run(true);

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
