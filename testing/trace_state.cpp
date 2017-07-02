#include "trace_state.hpp"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <sstream>

void split(const std::string& s, char delimiter, std::vector<std::string>& tokens)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter))
        tokens.push_back(item);
}

TraceManager::TraceManager(const std::list<TraceState>& trace_list, const std::vector<TraceInput>& inputs_by_frame, uint32_t frame) :
    traces(trace_list), inputs(inputs_by_frame), max_frame(frame)
{
    for (const auto& trace : traces)
        traces_by_frame[trace.frame] = trace;
}

const TraceState* TraceManager::GetState(uint32_t frame) const
{
    if (frame > max_frame)
        return nullptr;

    if (traces_by_frame.find(frame) != traces_by_frame.end())
        return &traces_by_frame.at(frame);

    std::map<uint32_t, TraceState>::const_iterator it = traces_by_frame.lower_bound(frame);
    it--;

    return &it->second;
}

const TraceInput* TraceManager::GetInput(uint32_t frame) const
{
    if (frame > max_frame)
        return nullptr;

    return &inputs[frame];
}

TraceManager read_trace_file(const std::string& filename)
{
    std::list<TraceState> traces;
    std::vector<TraceInput> inputs;
    std::string line;
    uint32_t frame = -1;
    uint16_t input;
    uint8_t selector_x, selector_y;
    std::ifstream file(filename.c_str());

    while (!file.eof()) {
        TraceState state;
        uint16_t address = 0;
        uint16_t start = 0;
        uint8_t value = 0;
        uint8_t x, y;
        std::getline(file, line);
        if (file.eof()) break;

        if (line == "frame start")
        {
            frame++;
            std::string strinput;
            std::getline(file, strinput);
            assert(strinput.find("input") != std::string::npos);
            {
                std::vector<std::string> tokens;
                split(strinput, ' ', tokens);
                uint8_t hi = strtol(tokens[1].c_str(), nullptr, 16);
                uint8_t lo = strtol(tokens[2].c_str(), nullptr, 16);
                input = hi << 8 | lo;
            }
            std::string selector;
            std::getline(file, selector);
            assert(selector.find("selector") != std::string::npos);
            {
                std::vector<std::string> tokens;
                split(selector, ' ', tokens);
                // off by 1
                selector_x = atoi(tokens[1].c_str()) - 1;
                // off by 3
                selector_y = atoi(tokens[2].c_str()) - 3;
            }
            inputs.emplace_back(frame, input);
            continue;
        }

        std::vector<std::string> address_value;
        split(line, ' ', address_value);
        address = strtol(address_value[0].c_str(), nullptr, 16);
        value = strtol(address_value[1].c_str(), nullptr, 16);

        if (address >= STATE_START && address <= STATE_END)
            start = address - STATE_START;
        if (address >= PANEL_START && address <= PANEL_END)
            start = address - PANEL_START;

        y = start / 2 / 8;
        x = start / 2 % 8;

        std::vector<uint32_t> panels;
        for (unsigned int i = 0; i < 13; i++)
        {
            std::getline(file, line);
            std::vector<std::string> values;
            split(line, ' ', values);
            for (const auto& val : values)
                panels.emplace_back(strtol(val.c_str(), nullptr, 16));
        }

        state.address = address;
        state.value = value;
        state.frame = frame;
        state.panels = std::move(panels);
        state.y = y;
        state.x = x;
        state.selector_x = selector_x;
        state.selector_y = selector_y;

        std::getline(file, line);
        std::getline(file, line);

        traces.emplace_back(state);
    }

    return TraceManager(traces, inputs, frame);
}
