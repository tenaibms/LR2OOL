#include "replayfix.h"
#include "hooks/hooks.h"
#include "config/config.h"

#include <LR2Bindings.hpp>

ReplayFix::ReplayFix()
{
    save_replay_hook = safetyhook::create_inline(reinterpret_cast<void*>(m_offsets.save_replay), reinterpret_cast<void*>(OnSaveReplay));
}

int __cdecl ReplayFix::OnSaveReplay(uint32_t* replay, void* song_md5, void* ArgList)
{
    ReplayFix& replay_fix = hooks::replay_fix;
    uint8_t* current_gauge = reinterpret_cast<uint8_t*>(LR2::pGame->config.play.gaugeOption);
    uint8_t* replay_gauge = reinterpret_cast<uint8_t*>(replay + replay_fix.m_offsets.replay_guage);

    if (replay_fix.m_enabled) {
        *replay_gauge = *current_gauge;
    }
    return replay_fix.save_replay_hook.ccall<int>(replay, song_md5, ArgList);
}
