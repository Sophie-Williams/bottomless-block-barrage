#define BOOST_TEST_MAIN
#if !defined(WINDOWS)
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/auto_unit_test.hpp>
#include "replay_simulation.hpp"

#define RED "\033[0;31m"
#define OFF "\033[0m"

bool RiseCounterCheck(uint16_t trace, uint16_t table, uint16_t speed)
{
    //printf("%x %x %x\n", trace, table, ((trace & 0xFFF) + (table & 0xFFF)));
    return std::abs(0xFFF - ((trace & 0xFFF) + (table & 0xFFF))) <= 0;
}

bool PanelCheck(uint32_t state, Panel& panel)
{
    return /*((panel.is_pending_match() && (state & 0x200000) == 0x200000) ||
           (panel.is_matched() && (state & 0x201000) == 0x201000) ||
           (panel.is_removed() && (state & 0xa01000) == 0xa01000) ||
           (panel.is_match_end() && state == 0) ||
           (panel.is_idle()) ||
           (panel.is_falling()) ||
           (panel.is_pending_fall()) ||
           (panel.is_fall_end()) ||
           (panel.is_fell_idle()) ||
           (panel.is_swapped()) ||
           (panel.is_swapping()) ||
           (panel.is_bottom()) ) &&*/
           ((state & 0xF) == (int)panel.get_value());
}

bool VerifyState(const FrameState& state, const PanelTable& table)
{
    const auto& panels = table.get_panels();
    const auto& next = table.get_next();
    //if (state.timeout != table.get_timeout() && state.timeout != 0 && state.timeout > table.get_timeout())
    //    return false;
    if (state.rise + table.get_rise() != 16)
        return false;
    if (!RiseCounterCheck(state.counter, table.get_rise_counter(), state.speed))
        return false;

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
        if (!PanelCheck(state.panels[i], panel))
            return false;
    }
    return true;
}

void PrintDiff(const FrameState& state, const PanelTable& table, uint32_t frame)
{
    const Input& input = state.input;

    printf("Mismatch found!\n");

    printf("frame: %d\n", frame);
    printf("input: %x\n", input.value());
    printf("selector: %d %d\n", state.y, state.x);
    printf("score: %d\n", state.score);
    printf("level: %d\n", state.level);
    printf("next: %d\n", state.next);
    printf("combo: %d\n", state.combo);
    printf("chain: %d vs %d\n", state.chain, table.get_chain());
    {
        bool failed = (state.timeout != table.get_timeout() && state.timeout != 0 && state.timeout > table.get_timeout());
        if (failed) printf("%s", RED);
        printf("timeout: %d vs %d\n", state.timeout, table.get_timeout());
        if (failed) printf("%s", OFF);
    }

    {
        bool failed = !RiseCounterCheck(state.counter, table.get_rise_counter(), state.speed);
        if (failed) printf("%s", RED);
        printf("rise counter: %x vs %x\n", state.counter, table.get_rise_counter());
        if (failed) printf("%s", OFF);
    }
    {
        bool failed = state.rise + table.get_rise() != 16;
        if (failed) printf("%s", RED);
        printf("rise: %d vs %d\n", state.rise, table.get_rise());
        if (failed) printf("%s", OFF);
    }
    printf("speed: %x\n", state.speed);
    for (const auto& fmstate : state.match_states)
        printf("match: %d %d %d %d %d\n", fmstate.pending, fmstate.blink, fmstate.matched, fmstate.removed, fmstate.deleted);

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
            bool failed = !PanelCheck(state.panels[j + i * 6], panel);
            printf("%s%012lx%s ", failed ? RED : "", state.panels[j + i * 6], failed ? OFF : "");
        }
        printf("\t");

        for (unsigned int j = 0; j < 6; j++)
        {
            Panel panel;
            if (i < 12)
                panel = table.get(i, j);
            else
                panel = table.get_next()[j];

            bool failed = !PanelCheck(state.panels[j + i * 6], panel);
            printf("%s%04x%02x%02x%s ", failed ? RED : "", panel.get_countdown(), panel.get_state(), panel.get_value(), failed ? OFF : "");
            if (failed) missed++;
        }

        printf("\n");
    }
    printf("Found %d mismatches in state\n", missed);
    printf("\n\n");
}

bool CheckState(const FrameState& state, const PanelTable& table, uint32_t frame)
{
    static bool failed_already = false;
    if (!VerifyState(state, table))
    {
        PrintDiff(state, table, frame);
        if (failed_already)
            BOOST_REQUIRE(0);
        failed_already = true;
        return false;
    }
    failed_already = false;
    return true;
}

void RunAndVerifyFrames(const std::string& frames_path, const std::string& skip_path)
{
    PanelSpeedSettings easy_speed_settings = {3, 11, 1, 46, 25, 9, FALL_ANIMATION_FRAMES};
    FrameReplaySimulation simulation(read_frames_file(frames_path), easy_speed_settings, read_skip_file(skip_path));

    simulation.AddStepCallback(CheckState);
    simulation.Run();
}


BOOST_AUTO_TEST_CASE(TestFrames)
{
    RunAndVerifyFrames("testdata/timeattack.frames", "testdata/timeattack.skip");
    RunAndVerifyFrames("testdata/slowtimeattack.frames", "testdata/slowtimeattack.skip");
}
