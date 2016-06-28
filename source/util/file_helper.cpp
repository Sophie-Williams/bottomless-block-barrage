#include "file_helper.hpp"

extern "C"
{
    #include <dirent.h>
}

std::vector<std::string> dir_entries(const std::string& path)
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

    return ret;
}
