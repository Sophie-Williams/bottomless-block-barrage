#define BOOST_TEST_MAIN
#if !defined(WINDOWS)
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/auto_unit_test.hpp>
#include "replay_simulation.hpp"

#define RED "\033[0;31m"
#define OFF "\033[0m"

bool VerifyState(const TraceState* state_ptr, const PanelTable& table)
{
    BOOST_REQUIRE(state_ptr != nullptr);
    const TraceState& state = *state_ptr;
    const auto& panels = table.get_panels();
    const auto& next = table.get_next();
    for (unsigned int i = 0; i < state.panels.size(); i++)
    {
        // Risen panel
        if (state.panels[i] == 0x00FF00FF)
            continue;
        Panel panel;
        if (i < 72)
            panel = panels[i];
        else
            panel = next[i - 72];
        if ((state.panels[i] & 0xF) != (int)panel.get_value())
            return false;
    }
    return true;
}

void PrintDiff(const TraceState* state_ptr, const PanelTable& table, const TraceInput* input_ptr, uint32_t frame)
{
    BOOST_REQUIRE(state_ptr != nullptr && input_ptr != nullptr);
    const TraceState& trace = *state_ptr;
    const TraceInput& input = *input_ptr;

    printf("Mismatch found!\nframe: %d\ninput: %x\naddress: %04x (%d %d) = %02x\nselector: (%d %d)\n", frame, input.value(), trace.address, trace.y, trace.x, trace.value, trace.selector_y, trace.selector_x);
    int missed = 0;
    for (unsigned int i = 0; i < 13; i++)
    {
        for (unsigned int j = 0; j < 6; j++)
        {
            Panel panel;
            if (i < 12)
                panel = table.get(i, j);
            else
                panel = table.get_next()[j];
            bool failed = panel.get_value() != (trace.panels[j + i * 6] & 0xF);
            printf("%s%08x%s ", failed ? RED : "", trace.panels[j + i * 6], failed ? OFF : "");
        }
        printf("\t");

        for (unsigned int j = 0; j < 6; j++)
        {
            Panel panel;
            if (i < 12)
                panel = table.get(i, j);
            else
                panel = table.get_next()[j];

            bool failed = panel.get_value() != (trace.panels[j + i * 6] & 0xF);
            printf("%s%04x%02x%02x%s ", failed ? RED : "", panel.get_countdown(), panel.get_state(), panel.get_value(), failed ? OFF : "");
            if (failed) missed++;
        }

        printf("\n");
    }
    printf("Found %d mismatches in state\n", missed);
    printf("\n\n");
}


bool RunAndVerifyTrace(const std::string& trace_path)
{
    PanelSpeedSettings easy_speed_settings = {4, 11, 1, 45, 25, 9, FALL_ANIMATION_FRAMES};
    TraceReplaySimulation simulation(read_trace_file(trace_path), easy_speed_settings);

    while (!simulation.Finished())
    {
        const auto& state = simulation.GetTraceState();
        const auto& input = simulation.GetInput();
        uint32_t frame = simulation.GetFrame();

        simulation.Step();
        bool ok = VerifyState(state, simulation.GetPanelTable());
        if (!ok)
        {
            PrintDiff(state, simulation.GetPanelTable(), input, frame);
            return false;
        }
    }
    return true;
}

bool VerifyState(const FrameState& state, const PanelTable& table)
{
    const auto& panels = table.get_panels();
    const auto& next = table.get_next();
    for (unsigned int i = 0; i < state.panels.size(); i++)
    {
        // Risen panel
        if (state.panels[i] == 0x00FF00FF)
            continue;
        Panel panel;
        if (i < 72)
            panel = panels[i];
        else
            panel = next[i - 72];
        if ((state.panels[i] & 0xF) != (int)panel.get_value())
            return false;
    }
    return true;
}

void PrintDiff(const FrameState& state, const PanelTable& table, uint32_t frame)
{
    const TraceInput& input = state.input;

    printf("Mismatch found!\n");

    printf("frame: %d\n", frame);
    printf("input: %x\n", input.value());
    printf("selector: %d %d\n", state.y, state.x);
    printf("score: %d\n", state.score);
    printf("level: %d\n", state.level);
    printf("next: %d\n", state.next);
    printf("combo: %d\n", state.combo);
    printf("chain: %d vs %d\n", state.chain, table.get_chain());
    printf("timeout: %d vs %d\n", state.timeout, table.get_cooloff());
    printf("rise counter: %x vs %x\n", state.rise_counter, table.get_rise_counter());
    printf("rise: %d vs %d\n", state.rise, table.get_rise());
    printf("speed: %x\n", state.rise_speed);

    int missed = 0;
    for (unsigned int i = 0; i < 13; i++)
    {
        for (unsigned int j = 0; j < 6; j++)
        {
            Panel panel;
            if (i < 12)
                panel = table.get(i, j);
            else
                panel = table.get_next()[j];
            bool failed = panel.get_value() != (state.panels[j + i * 6] & 0xF);
            printf("%s%08x%s ", failed ? RED : "", state.panels[j + i * 6], failed ? OFF : "");
        }
        printf("\t");

        for (unsigned int j = 0; j < 6; j++)
        {
            Panel panel;
            if (i < 12)
                panel = table.get(i, j);
            else
                panel = table.get_next()[j];

            bool failed = panel.get_value() != (state.panels[j + i * 6] & 0xF);
            printf("%s%04x%02x%02x%s ", failed ? RED : "", panel.get_countdown(), panel.get_state(), panel.get_value(), failed ? OFF : "");
            if (failed) missed++;
        }

        printf("\n");
    }
    printf("Found %d mismatches in state\n", missed);
    printf("\n\n");
}


bool RunAndVerifyFrames(const std::string& frames_path)
{
    PanelSpeedSettings easy_speed_settings = {4, 11, 1, 45, 25, 9, FALL_ANIMATION_FRAMES};
    FrameReplaySimulation simulation(read_frames_file(frames_path), easy_speed_settings);

    while (!simulation.Finished())
    {
        const auto& state = simulation.GetState();
        uint32_t frame = simulation.GetFrame();

        simulation.Step();
        bool ok = VerifyState(state, simulation.GetPanelTable());
        if (!ok)
        {
            PrintDiff(state, simulation.GetPanelTable(), frame);
            return false;
        }
    }
    return true;
}

BOOST_AUTO_TEST_CASE(TestTraces)
{
    BOOST_REQUIRE(RunAndVerifyTrace("traces/swap.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/swapfall.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/swapfall2.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/match3.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/matchfall.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/match10.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/2xchain.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/antislip.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/rapidswap.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/skillchain_slip.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/skillchain_triangle.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/test1.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/tumori1.trace"));
}
