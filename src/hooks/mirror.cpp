#include "mirror.h"

void hooks::mirror::Install()
{
    mirror_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::mirror), [](safetyhook::Context& ctx) {
        if (enabled) current_opt == 1 ? ctx.ebx = current_opt : ctx.ebx = 0;
    });

    reset_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::reset), [](safetyhook::Context& ctx) {
        ctx.ebx = 0;
    });
}
