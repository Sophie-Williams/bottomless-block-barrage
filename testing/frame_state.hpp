#ifndef FRAME_STATE_HPP
#define FRAME_STATE_HPP

#include "trace_state.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct FrameState
{
    uint32_t frame;
    TraceInput input;
    uint8_t x;
    uint8_t y;
    uint32_t score;
    uint16_t level;
    uint16_t next;
    uint16_t combo;
    uint16_t chain;
    uint16_t timeout;
    uint16_t rise_counter;
    uint16_t rise;
    uint16_t rise_speed;
    std::vector<uint32_t> panels;
};

class FrameStateManager
{
public:
    FrameStateManager(const std::vector<FrameState>& _frames) : frames(_frames) {}
    const FrameState& GetState(uint32_t frame) const {return frames[frame];}
    const FrameState& GetInitialState() const {return frames[0];}
    const std::vector<FrameState>& GetTraces() const {return frames;}
    uint32_t GetFinalFrame() const {return frames.size();}
private:
    std::vector<FrameState> frames;
};

FrameStateManager read_frames_file(const std::string& filename);

#endif
