#include "sfw.hpp"
#include "font.hpp"
#include "window.hpp"
#include "windowskin.h"
#include <cstdlib>

Font* default_font;

void sfw_init()
{
    default_font = new Font("romfs:/font/Arial_Black.bff");
    Window::set_skin(windowskin);
}

void sfw_exit()
{
    delete default_font;
}
