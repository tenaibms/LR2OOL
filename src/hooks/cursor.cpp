#include "cursor.h"
#include "overlay/overlay.h"

int __cdecl hooks::cursor::ShowCursor(int enabled)
{
    if (overlay::open)
        return cursor_hook.call<int>(1);
    return cursor_hook.call<int>(enabled);
}

void hooks::cursor::Install()
{
    cursor_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::show_cursor), reinterpret_cast<void*>(ShowCursor));
}
