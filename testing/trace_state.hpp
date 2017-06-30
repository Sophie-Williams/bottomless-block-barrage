#ifndef TRACE_STATE_HPP
#define TRACE_STATE_HP

#include <list>
#include <map>
#include <string>
#include <vector>

constexpr uint16_t STATE_START = 0xDAE;
constexpr uint16_t STATE_END = 0xE7A;
constexpr uint16_t PANEL_START = 0xFAE;
constexpr uint16_t PANEL_END = 0x107A;

struct TraceState
{
    uint32_t frame;
    uint16_t input;
    uint16_t address;
    uint8_t value;
    uint8_t x;
    uint8_t y;
    std::vector<uint32_t> panels;
};

class TraceManager
{
public:
    TraceManager(std::list<TraceState>& traces);
    const TraceState& GetState(uint32_t frame) const;
    const std::list<TraceState>& GetTraces() const {return traces;}
private:
    std::map<uint32_t, TraceState> traces_by_frame;
    std::list<TraceState> traces;
};

TraceManager read_trace_file(const std::string& filename);

#endif
