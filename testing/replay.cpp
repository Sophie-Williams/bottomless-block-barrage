#include "replay_simulation.hpp"
#include <cassert>
#include <memory>
#include <string>

PanelSpeedSettings easy_speed_settings   = {3, 11, 1, 46, 25, 9, FALL_ANIMATION_FRAMES};

int main(int argc, char** argv)
{
    std::string trace = argv[1];

    FrameStateManager manager = read_frames_file(trace);
    FrameReplaySimulation simulation(manager, easy_speed_settings);
    simulation.Run(true);
}
