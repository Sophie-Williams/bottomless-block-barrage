#include <audio.hpp>
#include <vector>

std::vector<std::pair<std::string, std::string>> tracks = {
{"Demo.brstm", "Demo_Critical.brstm"},
{"Yoshi.brstm", "Yoshi_Critical.brstm"}
};

std::string get_track(const std::string& track)
{
    return "romfs:/audio/" + track;
}
