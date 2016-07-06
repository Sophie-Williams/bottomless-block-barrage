#include "puzzle_set.hpp"
#include <sstream>

std::string construct_puzzle_filename(const std::string& set, const std::string& stage, const std::string& level)
{
    std::stringstream str;
    str << "romfs:/puzzles/" << set << "/" << stage << "/" << level << ".bbb";
    return str.str();
}
