#include "cursor.h"
#include "hooks/hooks.h"
#include "overlay/overlay.h"

Cursor::Cursor()
{
    m_cursor_hook = safetyhook::create_inline(reinterpret_cast<void*>(m_offsets.show_cursor), reinterpret_cast<void*>(ShowCursor));
}

int __cdecl Cursor::ShowCursor(int enabled) 
{
    Cursor& cursor = hooks::cursor;

    if (overlay::open)
        return cursor.m_cursor_hook.call<int>(1);
    return cursor.m_cursor_hook.call<int>(enabled);
}