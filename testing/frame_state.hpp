#ifndef FRAME_STATE_HPP
#define FRAME_STATE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "input.hpp"

struct FrameTime
{
    uint8_t minutes;
    uint8_t seconds;
};

struct FrameState
{
    uint32_t frame;
    Input input;
    Input trigger;
    Input repeat;
    FrameTime time;
    FrameTime timer;
    uint8_t x;
    uint8_t y;
    uint32_t score;
    uint16_t level;
    uint16_t next;
    uint16_t combo;
    uint16_t chain;
    uint16_t timeout;
    uint16_t counter;
    uint16_t rise;
    uint16_t speed;
    uint8_t state;
    std::vector<uint32_t> panels;
};

class FrameStateManager
{
public:
    FrameStateManager(const FrameState& first, const std::vector<FrameState>& _frames) : initial(first), frames(_frames) {}
    const FrameState& GetState(uint32_t frame) const {return frames[frame];}
    const FrameState& GetInitialState() const {return initial;}
    const std::vector<FrameState>& GetTraces() const {return frames;}
    uint32_t GetFinalFrame() const {return frames.size();}
private:
    FrameState initial;
    std::vector<FrameState> frames;

};

FrameStateManager read_frames_file(const std::string& filename);

#endif
