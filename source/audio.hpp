#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string>

// Returns normal, critical track
std::pair<std::string, std::string> get_random_track();
std::string get_track(const std::string& track);


#endif

