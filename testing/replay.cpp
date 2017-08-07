#include "replay_simulation.hpp"
#include <cassert>
#include <memory>
#include <string>

PanelSpeedSettings easy_speed_settings   = {3, 11, 1, 46, 25, 9, FALL_ANIMATION_FRAMES};

int main(int argc, char** argv)
{
    std::map<uint32_t, uint32_t> frame_skip_map;

    if (argc == 3)
        frame_skip_map = read_skip_file(argv[2]);

    FrameStateManager manager = read_frames_file(argv[1]);
    FrameReplaySimulation simulation(manager, easy_speed_settings, frame_skip_map);
    simulation.Run(true);
}
