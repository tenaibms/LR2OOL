#include "replayfix.h"

int __cdecl hooks::replayfix::SaveReplay(uint32_t* replay, int a2, void* ArgList)
{
    if (enabled) {
        uint8_t* replay_gauge = reinterpret_cast<uint8_t*>(*replay + offsets::save_replay);
        if (*replay_gauge != *current_gauge) {
            *replay_gauge = *current_gauge;
        }
    }
    return save_replay_hook.ccall<int>(replay, a2, ArgList);
}

void hooks::replayfix::Install()
{
    save_replay_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::save_replay), reinterpret_cast<void*>(SaveReplay));
}
