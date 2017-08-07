#ifndef FRAME_STATE_HPP
#define FRAME_STATE_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "input.hpp"

struct FrameTime
{
    uint8_t minutes;
    uint8_t seconds;
};

struct FrameMatchState
{
    uint16_t pending;
    bool blink;
    uint16_t matched;
    uint16_t removed;
    uint16_t deleted;
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
    std::vector<uint8_t> states;
    std::vector<FrameMatchState> match_states;
    std::vector<uint64_t> panels;
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

std::map<uint32_t, uint32_t> read_skip_file(const std::string& filename);
FrameStateManager read_frames_file(const std::string& filename);

#endif
