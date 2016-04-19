#include <3ds.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sf2d.h>
#include "game_state.hpp"

std::unique_ptr<GameState> state;

int main()
{
    sf2d_init();
    sf2d_set_clear_color(RGBA8(0x0, 0x0, 0xFF, 0xFF));
    sf2d_set_3D(0);

    GameState::Options options;
    options.rows = 11;
    options.columns = 6;
    options.colors = 6;
    state.reset(new GameState(options));

    while (aptMainLoop())
    {
        hidScanInput();
        u32 held = hidKeysHeld();

        if (held & KEY_START)
            break;

        state->Update();
        state->Render();

        sf2d_swapbuffers();
    }

    sf2d_fini();
    return 0;
}
