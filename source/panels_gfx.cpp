#include "panels_gfx.hpp"

std::map<std::string, PanelGfxDescription> panel_sets =
{
    {"srktiberious_set",      PanelGfxDescription("srktiberious_set",      true,  "srktiberious_set", 0, 0)},
    {"wolf_bunny_futuristic", PanelGfxDescription("wolf_bunny_futuristic", false, "wolf_bunny_futuristic", 1, 0)},
    {"wolf_bunny_glossy",     PanelGfxDescription("wolf_bunny_glossy",     false, "wolf_bunny_glossy", 2, 0)},
    {"wolf_bunny_ppc_set",    PanelGfxDescription("wolf_bunny_ppc_set",    false, "wolf_bunny_ppc_set", 3, 0)},
    {"wolf_bunny_set_1",      PanelGfxDescription("wolf_bunny_set_1",      false, "wolf_bunny_set", 4, 0)},
    {"wolf_bunny_set_2",      PanelGfxDescription("wolf_bunny_set_2",      false, "wolf_bunny_set", 4, 1)},
    {"wolf_bunny_set_3",      PanelGfxDescription("wolf_bunny_set_3",      false, "wolf_bunny_set", 4, 2)},
    {"wolf_bunny_set_4",      PanelGfxDescription("wolf_bunny_set_4",      false, "wolf_bunny_set", 4, 3)},
    {"wolf_bunny_set_5",      PanelGfxDescription("wolf_bunny_set_5",      false, "wolf_bunny_set", 4, 4)},
    {"wolf_bunny_set_6",      PanelGfxDescription("wolf_bunny_set_6",      false, "wolf_bunny_set", 4, 5)},
    {"wolf_bunny_set_7",      PanelGfxDescription("wolf_bunny_set_7",      false, "wolf_bunny_set", 4, 6)},
    {"wolf_bunny_set_8",      PanelGfxDescription("wolf_bunny_set_8",      false, "wolf_bunny_set", 4, 7)},
    {"wolf_bunny_set_9",      PanelGfxDescription("wolf_bunny_set_9",      false, "wolf_bunny_set", 4, 8)},
    {"wolf_bunny_set_10",     PanelGfxDescription("wolf_bunny_set_10",     false, "wolf_bunny_set", 4, 9)},
    {"wolf_bunny_set_11",     PanelGfxDescription("wolf_bunny_set_11",     false, "wolf_bunny_set", 4, 10)},
    {"wolf_bunny_set_12",     PanelGfxDescription("wolf_bunny_set_12",     false, "wolf_bunny_set", 4, 11)},
    {"wolf_bunny_set_13",     PanelGfxDescription("wolf_bunny_set_13",     false, "wolf_bunny_set", 4, 12)},
    {"wolf_bunny_set_14",     PanelGfxDescription("wolf_bunny_set_14",     false, "wolf_bunny_set", 4, 13)},
    {"wolf_bunny_set_15",     PanelGfxDescription("wolf_bunny_set_15",     false, "wolf_bunny_set", 4, 14)},
    {"wolf_bunny_set_16",     PanelGfxDescription("wolf_bunny_set_16",     false, "wolf_bunny_set", 4, 15)},
    {"wolf_bunny_ta_set",     PanelGfxDescription("wolf_bunny_ta_set",     true,  "wolf_bunny_ta_set", 5, 0)},
    {"wolf_bunny_ta_set_2",   PanelGfxDescription("wolf_bunny_ta_set_2",   false, "wolf_bunny_ta_set_2", 6, 0)},
    //{"wolf_bunny_dragon_ball", PanelGfxDescription("wolf_bunny_dragon_ball, false)},
};

PanelGfxDescription default_set = panel_sets["srktiberious_set"];

PanelGfxDescription::PanelGfxDescription(const std::string& gfx_rel_path, bool unmatchable, const char* panel_group, int i, int si) :
    include_unmatchable(unmatchable), group(panel_group), index(i), sub_index(si)
{
    gfx = "romfs:/graphics/panels/" + gfx_rel_path + ".png";
}
