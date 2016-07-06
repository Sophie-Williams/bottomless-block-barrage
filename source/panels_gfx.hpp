#ifndef PANELS_GFX_HPP
#define PANELS_GFX_HPP

#include <map>
#include <string>

#define PANEL_SIZE 16

struct PanelGfxDescription
{
    PanelGfxDescription() {}
    PanelGfxDescription(const std::string& gfx_rel_path, bool unmatchable, const char* group);
    std::string gfx;
    bool include_unmatchable;
    // Group marks this panel graphics as a recolor
    const char* group;
};

extern std::map<std::string, PanelGfxDescription> panel_sets;
extern PanelGfxDescription default_set;

#endif

