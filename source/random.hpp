#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdlib>

inline int randomInt(int max)
{
    return rand() / (RAND_MAX / max + 1);
}

inline int randomInt(int start, int end)
{
    return randomInt(end - start + 1) + start;
}

#endif
