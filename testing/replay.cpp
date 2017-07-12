#include "trace_state.hpp"
#include "replay_simulation.hpp"
#include <cassert>
#include <memory>
#include <string>

PanelSpeedSettings easy_speed_settings   = {4, 11, 1, 45, 25, 9, FALL_ANIMATION_FRAMES};

int main(int argc, char** argv)
{
    std::string trace = argv[1];

    std::unique_ptr<ReplaySimulation> simulation;
    if (trace.find(".trace") != std::string::npos)
    {
        TraceManager manager = read_trace_file(trace);
        simulation.reset(new TraceReplaySimulation(manager, easy_speed_settings));
    }
    else if (trace.find(".frames") != std::string::npos)
    {
        FrameStateManager manager = read_frames_file(trace);
        simulation.reset(new FrameReplaySimulation(manager, easy_speed_settings));
    }
    assert(simulation != nullptr);
    simulation->Run(true);
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
