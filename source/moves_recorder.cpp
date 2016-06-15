#include "moves_recorder.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#define MR_MAJOR_VERSION 0
#define MR_MINOR_VERSION 1

const std::string generate_filename()
{
    char buffer[128];
    /*time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    strftime(buffer, sizeof(buffer), "/bbb-moves/%Y-%m-%d-%X", &tstruct);*/
    sprintf(buffer, "/bbb-moves/%ld", osGetTime());
    return buffer;

}

void MovesRecorder::clear()
{
    moves.clear();
}

void MovesRecorder::add(u32 frame, u32 trigger, u32 held)
{
    if (!moves.empty())
    {
        Move& move = moves.back();
        if (move.frame == frame + 1 && move.trigger == trigger && move.held == held)
        {
            move.frames++;
            return;
        }
    }
    moves.emplace_back(frame, trigger, held);
}

bool MovesRecorder::save(const std::string& specific_filename)
{
    const std::string filename = generate_filename();

    std::ofstream file(specific_filename.empty() ? filename.c_str() : specific_filename.c_str(), std::ios::binary);
    if (!file.good()) return false;
    u32 size = moves.size();

    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(&seed), sizeof(seed));
    file.write(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
    file.write(reinterpret_cast<char*>(&level), sizeof(level));
    for (u32 i = 0; i < moves.size(); i++)
    {
        const Move& move = moves[i];
        file.write(reinterpret_cast<const char*>(&move.frame), sizeof(u32));
        file.write(reinterpret_cast<const char*>(&move.trigger), sizeof(u32));
        file.write(reinterpret_cast<const char*>(&move.held), sizeof(u32));
        file.write(reinterpret_cast<const char*>(&move.frames), sizeof(u32));
    }
    file.close();
    return true;
}



bool MovesRecorder::load(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.good()) return false;
    u32 size = 0;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
    file.read(reinterpret_cast<char*>(&level), sizeof(level));
    for (u32 i = 0; i < size; i++)
    {
        Move move;
        file.read(reinterpret_cast<char*>(&move.frame), sizeof(u32));
        file.read(reinterpret_cast<char*>(&move.trigger), sizeof(u32));
        file.read(reinterpret_cast<char*>(&move.held), sizeof(u32));
        file.read(reinterpret_cast<char*>(&move.frames), sizeof(u32));
        moves.push_back(move);
    }
    file.close();
    return true;
}

void MovesRecorder::keys(u32& trigger, u32& held)
{
    if (index >= moves.size())
        return;

    Move& move = moves[index];
    if (move.frame <= frame && frame <= move.frame + move.frames)
    {
        trigger = move.trigger;
        held = move.held;
    }
}

void MovesRecorder::update()
{
    frame++;
    while (index < moves.size() && frame > moves[index].frame + moves[index].frames)
        index++;
}
