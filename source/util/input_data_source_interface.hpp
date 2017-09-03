#ifndef INPUT_DATA_SOURCE_INTERFACE_HPP
#define INPUT_DATA_SOURCE_INTERFACE_HPP

#ifdef _3DS
#include <3ds.h>
#else
typedef unsigned int u32;
#endif

class InputDataSourceInterface
{
public:
    InputDataSourceInterface() {}
    virtual ~InputDataSourceInterface() {}
    virtual u32 trigger() const = 0;
    virtual u32 held() const = 0;
    virtual void update() = 0;
};

#endif
