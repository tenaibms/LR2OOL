#pragma once
#include <cstdint>
#include <safetyhook.hpp>

class Input {
public:
	Input() = default;
	void Init();
private:
	struct {
		const uintptr_t get_mouse_input = 0x5392C0;
	} m_offsets;

	SafetyHookInline m_on_get_mouse_input_hook;

	static uint8_t OnGetMouseInput();
};

