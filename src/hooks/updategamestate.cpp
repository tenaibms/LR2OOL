#include "updategamestate.h"
#include "features/hiterror.h"
#include "hooks/srcnumber.h"

void hooks::updategamestate::Install()
{
    gamestate_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::change_gamestate), [](safetyhook::Context& ctx) {
        gamestate = static_cast<GAMESTATE>(ctx.eax);
        if (static_cast<int>(gamestate) == 13) gamestate = GAMESTATE::result;
        if (gamestate == GAMESTATE::playing) {
            /* manually setting this variable feels inconsistent given precense of helper functions */
            hiterror::open = true;
            hiterror::Reset();
            srcnumber::Reset();
        }
        else {
            hiterror::open = false;
        }
    });
}
