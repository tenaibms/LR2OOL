#pragma once
#include <safetyhook.hpp>

namespace hooks::updategamestate {
	namespace offsets {
		const inline uintptr_t change_gamestate = 0x431BB6;
	};
	/* todo: fill this out with other values */
	enum class GAMESTATE {
		select = 2,
		decide = 3,
		playing = 4,
		result = 5
	};

	inline GAMESTATE gamestate = GAMESTATE::select;

	inline SafetyHookMid gamestate_hook;

	void Install();
}