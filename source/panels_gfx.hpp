#ifndef PANELS_GFX_HPP
#define PANELS_GFX_HPP
#include <map>
#include <string>

struct PanelGfxDescription
{
    const void* gfx;
    unsigned int panel_size;
    bool include_unmatchable;
    // Group marks this panel graphics as a recolor
    const char* group;
};

extern std::map<std::string, PanelGfxDescription> panel_sets;
extern PanelGfxDescription default_set;

#endif

