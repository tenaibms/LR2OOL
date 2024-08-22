#pragma once
#include <safetyhook.hpp>

namespace hooks::replayfix {
	namespace offsets {
		const inline uintptr_t save_replay = 0x4C09E0;
		const inline uintptr_t current_guage = 0x00FF840;
		const inline uintptr_t replay_guage = 0x50;
	}

	/* data pointers */
	inline uint8_t* current_gauge = reinterpret_cast<uint8_t*>(offsets::current_guage);

	inline bool enabled = false;
	inline SafetyHookInline save_replay_hook;
	int __cdecl SaveReplay(uint32_t* replay, int a2, void* ArgList);

	void Install();
}
