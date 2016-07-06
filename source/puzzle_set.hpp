#ifndef PUZZLE_SET_HPP
#define PUZZLE_SET_HPP

#include <map>
#include <string>
#include <vector>

struct PuzzleStage
{
    PuzzleStage() {}
    PuzzleStage(const std::string& n) : name(n) {}

    std::string name;
    std::vector<std::string> levels;
};

struct PuzzleSet
{
    PuzzleSet() {}
    PuzzleSet(const std::string& n) : name(n) {}
    std::vector<std::string> get_stages() const {return stage_names;}

    std::string name;
    std::vector<std::string> stage_names;
    std::map<std::string, PuzzleStage> stages;
};

std::string construct_puzzle_filename(const std::string& set, const std::string& stage, const std::string& level);

#endif

