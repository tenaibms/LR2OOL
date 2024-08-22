#pragma once
#include <safetyhook.hpp>

namespace hooks::cursor {
	namespace offsets {
		const inline uintptr_t show_cursor = 0x4D09E0;
	}

	inline SafetyHookInline cursor_hook;
	int __cdecl ShowCursor(int enabled);

	void Install();
}