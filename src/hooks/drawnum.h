#pragma once
#include <stdint.h>
#include <safetyhook.hpp>

namespace drawnum {
	namespace offsets {
		const inline uintptr_t draw_num = 0x49E060;
		const inline uintptr_t judge_y = 0x111F70;
		const inline uintptr_t ghost_setting = 0xFF8A0;
	}

	inline SafetyHookInline draw_hook;
	int hook_draw(void* drb, int src[17], int dst[11], void* T, int number, int x, int y);

	inline bool fs_toggle;
	inline bool pacemaker_toggle;

	void Install();
}