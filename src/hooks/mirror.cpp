#include "mirror.h"
#include "hooks/hooks.h"
#include "LR2Bindings.hpp"

Mirror::Mirror()
{
    m_random_p1_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.random_p1), [](safetyhook::Context& ctx) {
        if (hooks::mirror.m_enabled) ctx.ebx = IsSPMirror() || IsDPMirror();
        });

    m_random_p2_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.random_p2), [](safetyhook::Context& ctx) {
        if (hooks::mirror.m_enabled) ctx.ebx = IsDPMirror();
    });

    
    dp_flip_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.dp_flip), [](safetyhook::Context& ctx) {
        if (hooks::mirror.m_enabled) ctx.ebx = IsDPMirror();
    });

    reset_1_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.reset_1), [](safetyhook::Context& ctx) { ctx.ebx = 0; });
    reset_2_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.reset_2), [](safetyhook::Context& ctx) { ctx.ebx = 0; });
}

bool Mirror::IsSPMirror()
{
    const auto keymode = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].keymode;

    return (keymode == 5 || keymode == 7 || keymode == 9) &&
        LR2::pGame->config.play.random[0] == 1;
}

bool Mirror::IsDPMirror()
{
    const auto keymode = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].keymode;

    return (keymode == 14 || keymode == 10) &&
        LR2::pGame->config.play.random[0] == 1 &&
        LR2::pGame->config.play.random[1] == 1 &&
        LR2::pGame->config.play.dpflip;
}
