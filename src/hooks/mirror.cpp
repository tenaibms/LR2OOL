#include "mirror.h"
#include "hooks/hooks.h"

Mirror::Mirror()
{
    m_mirror_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.mirror), [](safetyhook::Context& ctx) {
        bool current_opt = *reinterpret_cast<bool*>(0x0FF848);
        if (hooks::mirror.m_enabled) current_opt == 1 ? ctx.ebx = current_opt : ctx.ebx = 0;
    });

    m_reset_hook = safetyhook::create_mid(reinterpret_cast<void*>(m_offsets.reset), [](safetyhook::Context& ctx) {
        ctx.ebx = 0;
    });
}
