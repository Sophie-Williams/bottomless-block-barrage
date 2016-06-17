#include "unnamed.hpp"
#include "font.hpp"
#include "window.hpp"
#include "windowskin.h"
#include <cstdlib>

Font* default_font;

void unnamed_init()
{
    default_font = new Font("romfs:/font/Arial_Black.bff");
    Window::set_skin(windowskin, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void unnamed_exit()
{
    delete default_font;
}
