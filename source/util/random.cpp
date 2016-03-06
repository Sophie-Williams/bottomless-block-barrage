#include "random.hpp"
#include <cstdlib>

int randomInt(int start, int end)
{
    return randomInt(end - start + 1) + start;
}

int randomInt(int max)
{
    return rand() / (RAND_MAX / max + 1);
}
