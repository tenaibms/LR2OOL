#pragma once
#include <stdint.h>
#include <safetyhook.hpp>

class Random {
public:
	Random() : random_1p(0), random_2p(0) {};
	void Init();

	unsigned random_1p;
	unsigned random_2p;
private:
	struct {
		const uintptr_t random_table = 0x4B483B;
		const uintptr_t table = 0x1B4;
	} m_offsets;

	SafetyHookMid random_table_hook;
	
	static void OnRandomTable(safetyhook::Context& ctx);
};