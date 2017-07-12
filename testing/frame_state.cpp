#include "frame_state.hpp"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <sstream>

// In trace_state.cpp
void split(const std::string& s, char delimiter, std::vector<std::string>& tokens);

FrameStateManager read_frames_file(const std::string& filename)
{
    std::vector<FrameState> frames;
    std::string line;
    std::ifstream file(filename.c_str());

    while (!file.eof())
    {
        FrameState state;
        std::getline(file, line);
        if (file.eof()) break;

        assert(line == "frame start");

        std::string strinput;
        std::getline(file, strinput);
        assert(strinput.find("input") != std::string::npos);
        {
            std::vector<std::string> tokens;
            split(strinput, ' ', tokens);
            uint8_t hi = strtol(tokens[1].c_str(), nullptr, 16);
            uint8_t lo = strtol(tokens[2].c_str(), nullptr, 16);
            state.input = TraceInput(frames.size(), hi << 8 | lo);
        }
        std::string selector;
        std::getline(file, selector);
        assert(selector.find("selector") != std::string::npos);
        {
            std::vector<std::string> tokens;
            split(selector, ' ', tokens);
            // off by 1
            state.x = atoi(tokens[1].c_str()) - 1;
            // off by 3
            state.y = atoi(tokens[2].c_str()) - 3;
        }
        std::string score;
        std::getline(file, score);
        assert(score.find("score") != std::string::npos);
        {
            state.score = strtol(score.c_str() + 6, nullptr, 16);
        }
        std::string level;
        std::getline(file, level);
        assert(level.find("level") != std::string::npos);
        {
            state.level = strtol(level.c_str() + 6, nullptr, 16);
        }
        std::string to_next_level;
        std::getline(file, to_next_level);
        assert(to_next_level.find("to_next_level") != std::string::npos);
        {
            state.next = strtol(to_next_level.c_str() + 14, nullptr, 16);
        }
        std::string combo;
        std::getline(file, combo);
        assert(combo.find("combo") != std::string::npos);
        {
            state.combo = strtol(combo.c_str() + 6, nullptr, 16);
        }
        std::string chain;
        std::getline(file, chain);
        assert(chain.find("chain") != std::string::npos);
        {
            state.chain = strtol(chain.c_str() + 6, nullptr, 16);
        }
        std::string timeout;
        std::getline(file, timeout);
        assert(timeout.find("timeout") != std::string::npos);
        {
            state.timeout = strtol(timeout.c_str() + 8, nullptr, 16);
        }
        std::string counter;
        std::getline(file, counter);
        assert(counter.find("rise_counter") != std::string::npos);
        {
            state.rise_counter = strtol(counter.c_str() + 13, nullptr, 16);
        }
        std::string rise;
        std::getline(file, rise);
        assert(rise.find("rise") != std::string::npos);
        {
            state.rise = strtol(rise.c_str() + 5, nullptr, 16);
        }
        std::string rise_speed;
        std::getline(file, rise_speed);
        assert(rise_speed.find("rise_speed") != std::string::npos);
        {
            state.rise_speed = strtol(rise_speed.c_str() + 11, nullptr, 16);
        }

        for (unsigned int i = 0; i < 13; i++)
        {
            std::getline(file, line);
            std::vector<std::string> values;
            split(line, ' ', values);
            for (const auto& val : values)
                state.panels.emplace_back(strtol(val.c_str(), nullptr, 16));
        }
        state.frame = frames.size();

        std::getline(file, line);
        std::getline(file, line);

        frames.push_back(state);
    }

    return FrameStateManager(frames);
}
