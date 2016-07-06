#include "file_helper.hpp"
#include <algorithm>

extern "C"
{
    #include <dirent.h>
}

std::string get_extension(const std::string& filename, std::string::size_type& idx)
{
    idx = filename.rfind('.');
    std::string extension;

    if (idx != std::string::npos)
        extension = filename.substr(idx + 1);
    else
        extension = "";

    return extension;
}

std::vector<std::string> dir_entries(const std::string& path, bool sort)
{
    std::vector<std::string> ret;
    DIR* d = opendir(path.c_str());
    if (!d) return ret;

    struct dirent* dir = readdir(d);
    while (dir != NULL)
    {
        const std::string& filename = dir->d_name;
        if (filename != "." && filename != "..")
            ret.push_back(filename);
        dir = readdir(d);
    }
    closedir(d);

    if (sort)
        std::sort(ret.begin(), ret.end());

    return ret;
}

std::vector<std::string> dir_filenames(const std::string& path, const std::string& ext, bool include_ext, bool sort)
{
    std::vector<std::string> entries = dir_entries(path, sort);
    std::vector<std::string> ret;
    std::string::size_type idx;
    for (const auto& entry : entries)
    {
        if (get_extension(entry, idx) == ext)
            ret.push_back(include_ext ? entry : entry.substr(0, idx));
    }
    return ret;
}
