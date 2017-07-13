#ifndef TRACE_STATE_HPP
#define TRACE_STATE_HPP

#include <list>
#include <map>
#include <string>
#include <vector>

constexpr uint16_t STATE_START = 0xDAE;
constexpr uint16_t STATE_END = 0xE7A;
constexpr uint16_t PANEL_START = 0xFAE;
constexpr uint16_t PANEL_END = 0x107A;

// Old style trace files which has each memory op logged.
struct TraceState
{
    uint32_t frame;
    uint16_t address;
    uint8_t value;
    uint8_t x;
    uint8_t y;
    uint8_t selector_x;
    uint8_t selector_y;
    std::vector<uint32_t> panels;
};

struct TraceInput
{
    TraceInput() : frame(0), input(0) {}
    TraceInput(uint32_t _frame, uint16_t _input) : frame(_frame), input(_input) {}

    bool button_a() const {return input & 0x80;}
    bool button_b() const {return input & 0x8000;}
    bool button_x() const {return input & 0x40;}
    bool button_y() const {return input & 0x4000;}
    bool button_l() const {return input & 0x10;}
    bool button_r() const {return input & 0x20;}

    bool button_start() const {return input & 0x1000;}
    bool button_select() const {return input & 0x2000;}

    bool button_left() const {return input & 0x200;}
    bool button_right() const {return input & 0x100;}
    bool button_up() const {return input & 0x800;}
    bool button_down() const {return input & 0x400;}
    uint16_t value() const {return input;}

    uint32_t frame;
    uint16_t input;
};

class TraceManager
{
public:
    TraceManager(const std::list<TraceState>& traces, const std::vector<TraceInput>& inputs, uint32_t max_frame);
    const TraceState* GetState(uint32_t frame) const;
    const TraceInput* GetInput(uint32_t frame) const;
    const TraceState& GetInitialState() const {return traces.front();}
    const std::list<TraceState>& GetTraces() const {return traces;}
    uint32_t GetFinalFrame() const {return max_frame;}
private:
    std::map<uint32_t, TraceState> traces_by_frame;
    std::list<TraceState> traces;
    std::vector<TraceInput> inputs;
    uint32_t max_frame;
};

TraceManager read_trace_file(const std::string& filename);

#endif
