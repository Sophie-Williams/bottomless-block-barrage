#include "panels_gfx.hpp"

std::map<std::string, PanelGfxDescription> panel_sets =
{
    {"srktiberious_set",      PanelGfxDescription("srktiberious_set",      true,  "srktiberious_set")},
    {"wolf_bunny_futuristic", PanelGfxDescription("wolf_bunny_futuristic", false, "wolf_bunny_futuristic")},
    {"wolf_bunny_glossy",     PanelGfxDescription("wolf_bunny_glossy",     false, "wolf_bunny_glossy")},
    {"wolf_bunny_ppc_set",    PanelGfxDescription("wolf_bunny_ppc_set",    false, "wolf_bunny_ppc_set")},
    {"wolf_bunny_set_1",      PanelGfxDescription("wolf_bunny_set_1",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_2",      PanelGfxDescription("wolf_bunny_set_2",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_3",      PanelGfxDescription("wolf_bunny_set_3",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_4",      PanelGfxDescription("wolf_bunny_set_4",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_5",      PanelGfxDescription("wolf_bunny_set_5",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_6",      PanelGfxDescription("wolf_bunny_set_6",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_7",      PanelGfxDescription("wolf_bunny_set_7",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_8",      PanelGfxDescription("wolf_bunny_set_8",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_9",      PanelGfxDescription("wolf_bunny_set_9",      false, "wolf_bunny_set")},
    {"wolf_bunny_set_10",     PanelGfxDescription("wolf_bunny_set_10",     false, "wolf_bunny_set")},
    {"wolf_bunny_set_11",     PanelGfxDescription("wolf_bunny_set_11",     false, "wolf_bunny_set")},
    {"wolf_bunny_set_12",     PanelGfxDescription("wolf_bunny_set_12",     false, "wolf_bunny_set")},
    {"wolf_bunny_set_13",     PanelGfxDescription("wolf_bunny_set_13",     false, "wolf_bunny_set")},
    {"wolf_bunny_set_14",     PanelGfxDescription("wolf_bunny_set_14",     false, "wolf_bunny_set")},
    {"wolf_bunny_set_15",     PanelGfxDescription("wolf_bunny_set_15",     false, "wolf_bunny_set")},
    {"wolf_bunny_set_16",     PanelGfxDescription("wolf_bunny_set_16",     false, "wolf_bunny_set")},
    {"wolf_bunny_ta_set",     PanelGfxDescription("wolf_bunny_ta_set",     true,  "wolf_bunny_ta_set")},
    {"wolf_bunny_ta_set_2",   PanelGfxDescription("wolf_bunny_ta_set_2",   false, "wolf_bunny_ta_set_2")},
    //{"wolf_bunny_dragon_ball", PanelGfxDescription("wolf_bunny_dragon_ball, false)},
};

PanelGfxDescription default_set = panel_sets["srktiberious_set"];

PanelGfxDescription::PanelGfxDescription(const std::string& gfx_rel_path, bool unmatchable, const char* panel_group) :
    include_unmatchable(unmatchable), group(panel_group)
{
    gfx = "romfs:/graphics/panels/" + gfx_rel_path + ".png";
}
