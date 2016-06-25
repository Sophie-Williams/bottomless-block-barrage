#include "panels_gfx.hpp"
#include "srktiberious_set.h"
#include "wolf_bunny_dragon_ball.h"
#include "wolf_bunny_futuristic.h"
#include "wolf_bunny_glossy.h"
#include "wolf_bunny_ppc_set.h"
#include "wolf_bunny_set_1.h"
#include "wolf_bunny_set_2.h"
#include "wolf_bunny_set_3.h"
#include "wolf_bunny_set_4.h"
#include "wolf_bunny_set_5.h"
#include "wolf_bunny_set_6.h"
#include "wolf_bunny_set_7.h"
#include "wolf_bunny_set_8.h"
#include "wolf_bunny_set_9.h"
#include "wolf_bunny_set_10.h"
#include "wolf_bunny_set_11.h"
#include "wolf_bunny_set_12.h"
#include "wolf_bunny_set_13.h"
#include "wolf_bunny_set_14.h"
#include "wolf_bunny_set_15.h"
#include "wolf_bunny_set_16.h"
#include "wolf_bunny_ta_set.h"
#include "wolf_bunny_ta_set_2.h"

std::map<std::string, PanelGfxDescription> panel_sets =
{
    {"srktiberious_set", {srktiberious_set, 16, true, "srktiberious_set"}},
    //{"wolf_bunny_dragon_ball", {wolf_bunny_dragon_ball, 16, false}},
    {"wolf_bunny_futuristic", {wolf_bunny_futuristic, 16, false, "wolf_bunny_futuristic"}},
    {"wolf_bunny_glossy", {wolf_bunny_glossy, 16, false, "wolf_bunny_glossy"}},
    {"wolf_bunny_ppc_set", {wolf_bunny_ppc_set, 16, false, "wolf_bunny_ppc_set"}},
    {"wolf_bunny_set_1", {wolf_bunny_set_1, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_2", {wolf_bunny_set_2, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_3", {wolf_bunny_set_3, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_4", {wolf_bunny_set_4, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_5", {wolf_bunny_set_5, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_6", {wolf_bunny_set_6, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_7", {wolf_bunny_set_7, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_8", {wolf_bunny_set_8, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_9", {wolf_bunny_set_9, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_10", {wolf_bunny_set_10, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_11", {wolf_bunny_set_11, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_12", {wolf_bunny_set_12, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_13", {wolf_bunny_set_13, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_14", {wolf_bunny_set_14, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_15", {wolf_bunny_set_15, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_set_16", {wolf_bunny_set_16, 16, false, "wolf_bunny_set"}},
    {"wolf_bunny_ta_set", {wolf_bunny_ta_set, 16, true, "wolf_bunny_ta_set"}},
    {"wolf_bunny_ta_set_2", {wolf_bunny_ta_set_2, 16, false, "wolf_bunny_ta_set_2"}},
};

PanelGfxDescription default_set = panel_sets["srktiberious_set"];
