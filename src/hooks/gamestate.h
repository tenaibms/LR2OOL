#pragma once
#include <safetyhook.hpp>

class GameState {
public:
	GameState();

	enum class StateList {
		select = 2,
		decide = 3,
		playing = 4,
		result = 5,
		key_config = 6,
		skin_select = 7
	};

	StateList m_current_state = StateList::select;
private:
	struct {
		const uintptr_t change_gamestate = 0x431BB6;
	} m_offsets;

	static void OnGameStateChange(safetyhook::Context& ctx);
	
	std::vector<SafetyHookMid> m_mid_hooks;
};