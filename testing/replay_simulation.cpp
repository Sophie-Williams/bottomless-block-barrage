#include "replay_simulation.hpp"

int combo_timeout[31][3] = {
 {   0,      0,     0},
 {   0,      0,     0},
 {   0,      0,     0},
 {   0,      0,     0},
 { 0x78,  0x5a,  0x3c},
 { 0x96,  0x78,  0x5a},
 { 0xb4,  0x96,  0x78},
 { 0xd2,  0xb4,  0x96},
 { 0xf0,  0xd2,  0xb4},
 {0x10e,  0xf0,  0xd2},
 {0x12c, 0x10e,  0xf0},
 {0x14a, 0x12c, 0x10e},
 {0x168, 0x14a, 0x12c},
 {0x168, 0x168, 0x14a},
 {0x1a4, 0x168, 0x168},
 {0x1c2, 0x1a4, 0x168},
 {0x1e0, 0x1c2, 0x1a4},
 {0x1fe, 0x1e0, 0x1c2},
 {0x21c, 0x1fe, 0x1e0},
 {0x23a, 0x21c, 0x1fe},
 {0x258, 0x23a, 0x21c},
 {0x276, 0x258, 0x23a},
 {0x294, 0x276, 0x258},
 {0x2b2, 0x294, 0x276},
 {0x2d0, 0x2b2, 0x294},
 {0x2ee, 0x2d0, 0x2b2},
 {0x30c, 0x2ee, 0x2d0},
 {0x32a, 0x30c, 0x2ee},
 {0x348, 0x32a, 0x30c},
 {0x366, 0x348, 0x32a},
 {0x384, 0x366, 0x348},
};

int chain_timeout[15][3] = {
{    0,     0,     0},
{    0,     0,     0},
{0x12c,  0xe1,  0x96},
{0x168, 0x11d,  0xd2},
{0x1a4, 0x159, 0x10e},
{0x1e0, 0x195, 0x14a},
{0x21c, 0x1d1, 0x186},
{0x258, 0x20d, 0x1c2},
{0x294, 0x249, 0x1fe},
{0x2d0, 0x285, 0x23a},
{0x30c, 0x2c1, 0x276},
{0x348, 0x2fd, 0x2b2},
{0x384, 0x339, 0x2ee},
{0x3c0, 0x375, 0x32a},
{0x3c0, 0x375, 0x32a},
};

// Per speed level this is the value to add to a counter on each frame.
// Once the counter is >= 0x1000 another counter represented the panels rise amount in pixels (hence called rise-counter)
// is incremented and 0x1000 is subtracted from the counter.
// Once the rise amount is 16 a rise happens and the rise-counter is reset to 0.
const std::map<int, int> speed_table
{
    {1,   0x0047},
    {2,   0x0044},
    {3,   0x0050},
    {4,   0x0055},
    {5,   0x0059},
    {6,   0x0061},
    {7,   0x0068},
    {8,   0x0070},
    {9,   0x0077},
    {10,  0x0084},
    {11,  0x0090},
    {12,  0x009a},
    {13,  0x00af},
    {14,  0x00bb},
    {15,  0x00c8},
    {16,  0x00d6},
    {17,  0x00e4},
    {18,  0x00f3},
    {19,  0x0104},
    {20,  0x0115},
    {21,  0x0128},
    {22,  0x013c},
    {23,  0x0151},
    {24,  0x0168},
    {25,  0x0181},
    {26,  0x019c},
    {27,  0x01b7},
    {28,  0x01d7},
    {29,  0x01fc},
    {30,  0x0226},
    {31,  0x0253},
    {32,  0x0288},
    {33,  0x02c0},
    {34,  0x02fa},
    {35,  0x0333},
    {36,  0x0381},
    {37,  0x03d2},
    {38,  0x0410},
    {39,  0x0469},
    {40,  0x04bd},
    {41,  0x051e},
    {42,  0x0572},
    {43,  0x05f4},
    {44,  0x0666},
    {45,  0x06eb},
    {46,  0x0750},
    {47,  0x07c1},
    {48,  0x0842},
    {49,  0x08d3},
    {50,  0x0924},
    {51,  0x097b},
    {52,  0x09d8},
    {53,  0x0a3d},
    {54,  0x0aaa},
    {55,  0x0b21},
    {56,  0x0ba2},
    {57,  0x0c30},
    {58,  0x0ccc},
    {59,  0x0d79},
    {60,  0x0e38},
    {61,  0x0f0f},
    {100, 0x1000},
};

// Number of Panels to match for next level
const std::map<int, int> level_table
{
    {1,    9},
    {6,   12},
    {8,   14},
    {10,  16},
    {15,  24},
    {16,  22},
    {17,  20},
    {18,  18},
    {20,  16},
    {30,  36},
    {31,  39},
    {100, 45},
};

int get_speed_for_level(int level)
{
    return speed_table.lower_bound(level)->second;
}

int get_panels_for_level(int level)
{
    return level_table.lower_bound(level)->second;
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
            for (int j = 0; j < 6; j++) {
                printf("%d ", state.panels[72 + j] & 0xFF);
                next.push_back((Panel::Type)(state.panels[72 + j] & 0xFF));
            }
            printf("\n");
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

FrameReplaySimulation::FrameReplaySimulation(const FrameStateManager& frame_manager, const PanelSpeedSettings& settings) :
     frame(0), table(nullptr), frames(frame_manager), level(1), next(0), x(0), y(0)
{
    const FrameState& initial = frames.GetInitialState();
    ReplayPanelSource* source = new ReplayPanelSource(get_panels(initial), get_next_panels(frames));
    PanelTable::Options opts;
    opts.source = source;
    opts.columns = 6;
    opts.rows = 12;
    opts.settings = settings;
    opts.type = PanelTable::Type::ENDLESS;
    table.reset(new PanelTable(opts));

    next = get_panels_for_level(level);
    table->set_speed(get_speed_for_level(level));
    x = initial.x;
    y = initial.y;
}

void FrameReplaySimulation::Step(bool debug)
{
    DoStep();
    if (frame < frames.GetFinalFrame())
    {
        for (const auto& callback : callbacks)
            callback(frames.GetState(frame), *table, frame);
        if (debug) Print();
    }
    frame++;
}

void FrameReplaySimulation::Run(bool debug)
{
    while (!Finished())
        Step(debug);
}

void FrameReplaySimulation::Print()
{

    const auto& state = frames.GetState(frame/*+1*/);
    printf("frame: %d\n", frame);
    printf("match info: %d %d %d\n", info.combo, info.chain, info.clink);
    printf("input: %x %x %x\n", state.input.value(), state.trigger.value(), state.repeat.value());
    printf("time: %d:%02d\n", state.time.minutes, state.time.seconds);
    printf("timer: %d:%02d\n", state.timer.minutes, state.timer.seconds);
    printf("selector: %d %d vs %d %d\n", state.y, state.x, y, x);
    printf("score: %d\n", state.score);
    printf("level: %d vs %d\n", state.level, level);
    printf("next: %d vs %d\n", state.next, next);
    printf("combo: %d vs %d\n", state.combo, info.combo);
    printf("chain: %d vs %d\n", state.chain, info.chain);
    printf("timeout: %d vs %d\n", state.timeout, table->get_timeout());
    printf("counter: %x vs %x\n", state.counter, table->get_rise_counter());
    printf("rise: %d vs %d\n", state.rise, table->get_rise());
    printf("speed: %x vs %x\n", state.speed, get_speed_for_level(level));

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

            printf("%04d%02x%02x ", panel.get_countdown(), panel.get_state(), panel.get_value());
        }

        printf("\n");
    }
    printf("\n\n");
}

void FrameReplaySimulation::DoStep()
{
    const auto& state = frames.GetState(frame);
    input_manager.update(state.input);

    y = state.y;
    x = state.x;
    /*if (input_manager.repeat_quick(Input::BUTTON_DOWN, 10, 10, 1))
        y = std::min(11, y + 1);
    else if (input_manager.repeat_quick(Input::BUTTON_LEFT, 10, 10, 1))
        x = std::max(0, x - 1);
    else if (input_manager.repeat_quick(Input::BUTTON_RIGHT, 10, 10, 1))
        x = std::min(4, x + 1);
    else if (input_manager.repeat_quick(Input::BUTTON_UP, 10, 10, 1))
        y = std::max(0, y - 1);

    if (table->is_rised())
        y = std::max(0, y - 1);*/

    info = table->update();
    next -= info.combo;
    if (next <= 0)
    {
        level++;
        next += get_panels_for_level(level);
        table->set_speed(get_speed_for_level(level));
    }

    if (input_manager.triggered(Input::BUTTON_A) || input_manager.triggered(Input::BUTTON_B))
        table->swap(y, x);

    if (input_manager.on(Input::BUTTON_L) || input_manager.on(Input::BUTTON_R))
        table->quick_rise();

    table->freeze(combo_timeout[info.combo][0]);
    if (info.fall_match)
        table->freeze(chain_timeout[info.chain][0]);
}

void FrameReplaySimulation::GetSelectorCoords(int& i, int& j) const
{
    i = y;
    j = x;
}
