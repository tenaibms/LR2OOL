#pragma once
#include <stdint.h>
#include <safetyhook.hpp>

namespace loadbms {
	namespace offsets {
		const uintptr_t reset = 0x4B0690;
		const uintptr_t sum = 0x4B32AD; /* no need for count when it is *always* called with sum */
		const uintptr_t random_table = 0x4B483B;
		const uintptr_t table = 0x1B4;
	}

	inline unsigned random_1p;
	inline unsigned random_2p;

	inline SafetyHookMid random_table_hook;

	void Install();
}