#include "frame_state.hpp"

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

FrameStateManager read_frames_file(const std::string& filename)
{
    std::vector<FrameState> frames;
    FrameState initial;
    std::string line;
    std::ifstream file(filename.c_str());

    uint32_t frame = 0;

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
            uint16_t input = strtol(tokens[1].c_str(), nullptr, 16);
            uint16_t trigger = strtol(tokens[2].c_str(), nullptr, 16);
            uint16_t repeat = strtol(tokens[3].c_str(), nullptr, 16);
            state.input = Input(input);
            state.trigger = Input(trigger);
            state.repeat = Input(repeat);
        }
        std::string time;
        std::getline(file, time);
        assert(time.find("time") != std::string::npos);
        {
            std::vector<std::string> tokens;
            split(time, ' ', tokens);
            state.time.minutes = strtol(tokens[1].c_str(), nullptr, 10);
            state.time.seconds = strtol(tokens[2].c_str(), nullptr, 10);
        }
        std::string timer;
        std::getline(file, timer);
        assert(timer.find("timer") != std::string::npos);
        {
            std::vector<std::string> tokens;
            split(timer, ' ', tokens);
            state.timer.minutes = strtol(tokens[1].c_str(), nullptr, 10);
            state.timer.seconds = strtol(tokens[2].c_str(), nullptr, 10);
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
            state.score = strtol(score.c_str() + 6, nullptr, 10);
        }
        std::string level;
        std::getline(file, level);
        assert(level.find("level") != std::string::npos);
        {
            state.level = strtol(level.c_str() + 6, nullptr, 10);
        }
        std::string next;
        std::getline(file, next);
        assert(next.find("next") != std::string::npos);
        {
            state.next = strtol(next.c_str() + 5, nullptr, 10);
        }
        std::string combo;
        std::getline(file, combo);
        assert(combo.find("combo") != std::string::npos);
        {
            state.combo = strtol(combo.c_str() + 6, nullptr, 10);
        }
        std::string chain;
        std::getline(file, chain);
        assert(chain.find("chain") != std::string::npos);
        {
            state.chain = strtol(chain.c_str() + 6, nullptr, 10);
        }
        std::string timeout;
        std::getline(file, timeout);
        assert(timeout.find("timeout") != std::string::npos);
        {
            state.timeout = strtol(timeout.c_str() + 8, nullptr, 10);
        }
        std::string rise;
        std::getline(file, rise);
        assert(rise.find("rise") != std::string::npos);
        {
            state.rise = strtol(rise.c_str() + 5, nullptr, 10);
        }
        std::string counter;
        std::getline(file, counter);
        assert(counter.find("counter") != std::string::npos);
        {
            state.counter = strtol(counter.c_str() + 8, nullptr, 16);
        }
        std::string rise_speed;
        std::getline(file, rise_speed);
        assert(rise_speed.find("speed") != std::string::npos);
        {
            state.speed = strtol(rise_speed.c_str() + 6, nullptr, 16);
        }
        std::string state_str;
        std::getline(file, state_str);
        assert(state_str.find("states") != std::string::npos);
        {
            std::vector<std::string> tokens;
            split(state_str, ' ', tokens);
            for (unsigned int i = 1; i < tokens.size(); i++)
                state.states.push_back(strtol(tokens[i].c_str(), nullptr, 10));
        }
        {
            std::vector<std::string> pending;
            std::vector<std::string> blink;
            std::vector<std::string> matched;
            std::vector<std::string> removed;
            std::vector<std::string> deleted;
            std::string line;

            std::getline(file, line);
            assert(line.find("pending") != std::string::npos);
            split(line, ' ', pending);

            std::getline(file, line);
            assert(line.find("blink") != std::string::npos);
            split(line, ' ', blink);

            std::getline(file, line);
            assert(line.find("match") != std::string::npos);
            split(line, ' ', matched);

            std::getline(file, line);
            assert(line.find("remove") != std::string::npos);
            split(line, ' ', removed);

            std::getline(file, line);
            assert(line.find("delete") != std::string::npos);
            split(line, ' ', deleted);

            for (unsigned int i = 0; i < pending.size(); i++)
            {
                FrameMatchState fmstate;
                fmstate.pending = atoi(pending[i].c_str());
                fmstate.blink = atoi(blink[i].c_str());
                fmstate.matched = atoi(matched[i].c_str());
                fmstate.removed = atoi(removed[i].c_str());
                fmstate.deleted = atoi(deleted[i].c_str());
                state.match_states.push_back(fmstate);
            }
        }

        for (unsigned int i = 0; i < 13; i++)
        {
            std::getline(file, line);
            std::vector<std::string> values;
            split(line, ' ', values);
            for (const auto& val : values)
                state.panels.push_back(strtoull(val.c_str(), nullptr, 16));
        }
        state.frame = frame;

        std::getline(file, line);
        std::getline(file, line);


        if (frame != 0)
            frames.push_back(state);

        if (frame == 0)
            initial = state;

        frame++;
    }

    return FrameStateManager(initial, frames);
}

std::map<uint32_t, uint32_t> read_skip_file(const std::string& filename)
{
    std::map<uint32_t, uint32_t> ret;

    std::string line;
    std::ifstream file(filename.c_str());
    while (!file.eof())
    {
        std::getline(file, line);
        if (line == "\n") continue;
        std::vector<std::string> tokens;
        split(line, ':', tokens);
        if (tokens.size() != 2) continue;

        uint32_t frame = atoi(tokens[0].c_str());
        uint32_t skip = atoi(tokens[1].c_str());

        ret[frame] = skip;
    }
    return ret;
}
