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
    for (unsigned int i = 0; i < state.panels.size(); i++)
    {
        if ((state.panels[i] & 0xF) != (int)table.panels[i].value)
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
            const auto& panel = table.get(i, j);
            bool failed = panel.value != (trace.panels[j + i * 6] & 0xF);
            printf("%s%08x%s ", failed ? RED : "", trace.panels[j + i * 6], failed ? OFF : "");
        }
        printf("\t");
        for (unsigned int j = 0; j < 6; j++)
        {
            const auto& panel = table.get(i, j);
            bool failed = panel.value != (trace.panels[j + i * 6] & 0xF);
            printf("%s%04x%04x%s ", failed ? RED : "", panel.state, panel.value, failed ? OFF : "");
            if (failed) missed++;
        }

        printf("\n");
    }
    printf("Found %d mismatches in state\n", missed);
    printf("\n\n");
}


bool RunAndVerifyTrace(const std::string& trace_path)
{
    ReplaySimulation simulation(read_trace_file(trace_path));

    while (!simulation.Finished())
    {
        bool ok = VerifyState(simulation.GetTraceState(), simulation.GetPanelTable());
        if (!ok)
        {
            PrintDiff(simulation.GetTraceState(), simulation.GetPanelTable(), simulation.GetInput(), simulation.GetFrame());
            return false;
        }
        simulation.Step();
    }
    return true;
}

BOOST_AUTO_TEST_CASE(TestTraces)
{
    BOOST_REQUIRE(RunAndVerifyTrace("traces/swap.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/rise.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/match3.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/swapfall.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/matchfall.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/skillchain_slip.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/skillchain_triangle.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/rapidswap.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/test1.trace"));
    BOOST_REQUIRE(RunAndVerifyTrace("traces/tumori1.trace"));
}
