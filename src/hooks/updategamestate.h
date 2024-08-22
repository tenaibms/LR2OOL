#pragma once
#include <safetyhook.hpp>

namespace hooks::updategamestate {
	namespace offsets {
		const inline uintptr_t change_gamestate = 0x431BB9;
	};
	/* todo: fill this out with other values */
	enum class GAMESTATE {
		playing = 2
	};

	inline SafetyHookMid gamestate_hook;

	void Install();
}