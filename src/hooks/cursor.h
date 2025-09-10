#pragma once
#include <safetyhook.hpp>

class Cursor {
public:
	Cursor() = default;
	void Init();
private:
	struct {
		const uintptr_t show_cursor = 0x4D09E0;
	} m_offsets;

	SafetyHookInline m_cursor_hook;
	static int __cdecl ShowCursor(int enabled);
};