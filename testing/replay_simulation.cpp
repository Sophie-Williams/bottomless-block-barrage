#include "replay_simulation.hpp"

std::vector<Panel::Type> get_panels(const TraceState& initial)
{
    std::vector<Panel::Type> panels(72, Panel::Type::EMPTY);
    for (unsigned int i = 0; i < panels.size(); i++)
        panels[i] = (Panel::Type) (initial.panels[i] & 0xFF);
    return panels;
}

std::vector<Panel::Type> get_next_panels(const TraceManager& trace_manager)
{
    std::vector<Panel::Type> next;

    bool table_has_risen = false;
    const TraceState& state = trace_manager.GetInitialState();
    for (int j = 0; j < 6; j++)
        next.push_back((Panel::Type)(state.panels[72 + j] & 0xFF));


    for (uint32_t i = 0; i < trace_manager.GetFinalFrame(); i++)
    {
        const TraceState* state_ptr = trace_manager.GetState(i);
        if (state_ptr == nullptr) continue;

        const auto& state = *state_ptr;
        if (table_has_risen)
        {
            for (int j = 0; j < 6; j++)
                next.push_back((Panel::Type)(state.panels[72 + j] & 0xFF));
            table_has_risen = false;
        }
        if (state.panels[72] == 0xFF00FF)
            table_has_risen = true;
    }
    return next;
}

std::vector<Panel::Type> get_panels(const FrameState& initial)
{
    std::vector<Panel::Type> panels(72, Panel::Type::EMPTY);
    for (unsigned int i = 0; i < panels.size(); i++)
        panels[i] = (Panel::Type) (initial.panels[i] & 0xFF);
    return panels;
}

std::vector<Panel::Type> get_next_panels(const FrameStateManager& frame_manager)
{
    std::vector<Panel::Type> next;

    bool table_has_risen = false;
    const FrameState& state = frame_manager.GetInitialState();
    for (int j = 0; j < 6; j++)
        next.push_back((Panel::Type)(state.panels[72 + j] & 0xFF));


    for (uint32_t i = 0; i < frame_manager.GetFinalFrame(); i++)
    {
        const auto& state = frame_manager.GetState(i);
        if (table_has_risen)
        {
            for (int j = 0; j < 6; j++)
                next.push_back((Panel::Type)(state.panels[72 + j] & 0xFF));
            table_has_risen = false;
        }
        if (state.panels[72] == 0xFF00FF)
            table_has_risen = true;
    }
    return next;
}

ReplayPanelSource::ReplayPanelSource(const std::vector<Panel::Type>& _board, const std::vector<Panel::Type>& _next) : PanelSource(12, 6), table(_board), next(_next), index(0)
{
}

std::vector<Panel::Type> ReplayPanelSource::line()
{
    std::vector<Panel::Type> line(columns, Panel::Type::EMPTY);
    if (index > (int)next.size())
        return line;

    for (int i = 0; i < columns; i++)
    {
        line[i] = next[index + i];
    }

    index += columns;

    return line;
}

void ReplaySimulation::Step()
{
    DoStep();
    frame++;
}

void ReplaySimulation::Run(bool debug)
{
    while (!Finished())
    {
        Step();
        if (debug) Print();
    }
}

TraceReplaySimulation::TraceReplaySimulation(const TraceManager& trace_manager, const PanelSpeedSettings& settings) : table(nullptr), traces(trace_manager), last_input(0, 0)
{
    ReplayPanelSource* source = new ReplayPanelSource(get_panels(trace_manager.GetInitialState()), get_next_panels(trace_manager));
    PanelTable::Options opts;
    opts.source = source;
    opts.columns = 6;
    opts.rows = 12;
    opts.settings = settings;
    opts.type = PanelTable::Type::ENDLESS;
    table.reset(new PanelTable(opts));
}

void TraceReplaySimulation::DoStep()
{
    const auto& trace_ptr = traces.GetState(frame);
    const auto& input_ptr = traces.GetInput(frame);
    if (trace_ptr == nullptr || input_ptr == nullptr) return;

    const auto& trace = *trace_ptr;
    const auto& input = *input_ptr;

    if (input.button_a() && !last_input.button_a())
        table->swap(trace.selector_y, trace.selector_x);

    table->update(0x47);

    last_input = input;
}

void TraceReplaySimulation::Print()
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
            Panel panel;
            if (i < 12)
                panel = table->get(i, j);
            else
                panel = table->get_next()[j];

            printf("%04x%02x%02x ", panel.get_countdown(), panel.get_state(), panel.get_value());
        }

        printf("\n");
    }
    printf("\n\n");
}

FrameReplaySimulation::FrameReplaySimulation(const FrameStateManager& frame_manager, const PanelSpeedSettings& settings): table(nullptr), frames(frame_manager), last_input(0, 0)
{
    ReplayPanelSource* source = new ReplayPanelSource(get_panels(frames.GetInitialState()), get_next_panels(frames));
    PanelTable::Options opts;
    opts.source = source;
    opts.columns = 6;
    opts.rows = 12;
    opts.settings = settings;
    opts.type = PanelTable::Type::ENDLESS;
    table.reset(new PanelTable(opts));
}

void FrameReplaySimulation::Print()
{
    const auto& state = frames.GetState(frame-1);
    printf("frame: %d\n", frame - 1);
    printf("input: %x\n", state.input.value());
    printf("selector: %d %d\n", state.y, state.x);
    printf("score: %d\n", state.score);
    printf("level: %d\n", state.level);
    printf("next: %d\n", state.next);
    printf("combo: %d\n", state.combo);
    printf("chain: %d\n", state.chain);
    printf("timeout: %d\n", state.timeout);
    printf("rise counter: %x\n", state.rise_counter);
    printf("rise: %d\n", state.rise);
    printf("speed: %x\n", state.rise_speed);

    for (unsigned int i = 0; i < 13; i++)
    {
        for (unsigned int j = 0; j < 6; j++)
        {
            printf("%08x ", state.panels[j + i * 6]);
        }
        printf("\t\t");
        for (unsigned int j = 0; j < 6; j++)
        {
            Panel panel;
            if (i < 12)
                panel = table->get(i, j);
            else
                panel = table->get_next()[j];

            printf("%04x%02x%02x ", panel.get_countdown(), panel.get_state(), panel.get_value());
        }

        printf("\n");
    }
    printf("\n\n");
}

void FrameReplaySimulation::DoStep()
{
    const auto& state = frames.GetState(frame);
    const auto& input = state.input;

    if (input.button_a() && !last_input.button_a())
        table->swap(state.y, state.x);

    table->update(0x47);

    last_input = input;
}
