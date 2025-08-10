#pragma once
#include <safetyhook.hpp>

class SkinMisc {
private:
	struct {
		const uintptr_t draw_ln = 0x407A83;
		const uintptr_t switch_statement_27 = 0x413F64;
		const uintptr_t switch_statement_28 = 0x413F68;
		const uintptr_t cmp = 0x4135AD;
	} offsets;

	template <typename T>
	void Patch(uintptr_t offset, T value);

	static void OnDrawLN(safetyhook::Context& ctx);
	SafetyHookMid draw_ln_hook;
public:
	SkinMisc();
	~SkinMisc();
};
