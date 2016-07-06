#ifndef FILE_HELPER_HPP
#define FILE_HELPER_HPP

#include <string>
#include <vector>

std::vector<std::string> dir_entries(const std::string& path, bool sort = false);
// Returns all filenames that match extension in directory Optionally removing the extension.
std::vector<std::string> dir_filenames(const std::string& path, const std::string& ext, bool include_ext = false, bool sort = false);

#endif


