#include "moves_recorder.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

const std::string generate_filename()
{
    char buffer[128];
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    strftime(buffer, sizeof(buffer), "/bbb-moves/%Y-%m-%d-%X", &tstruct);
    return buffer;
}

void MovesRecorder::add(unsigned int frame, unsigned int trigger, unsigned int held)
{
    moves.emplace_back(frame, trigger, held);
}

void MovesRecorder::clear()
{
    moves.clear();
}

void MovesRecorder::save()
{
    const std::string filename = generate_filename();
    std::ofstream file(filename.c_str(), std::ios::binary);
    unsigned int size = moves.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(&seed), sizeof(seed));
    for (unsigned int i = 0; i < moves.size(); i++)
    {
        const Move& move = moves[i];
        file.write(reinterpret_cast<const char*>(&move.frame), sizeof(unsigned int));
        file.write(reinterpret_cast<const char*>(&move.trigger), sizeof(unsigned int));
        file.write(reinterpret_cast<const char*>(&move.held), sizeof(unsigned int));
    }
    file.close();
}
