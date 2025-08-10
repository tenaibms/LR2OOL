#pragma once
#include <safetyhook.hpp>

class SkinMisc {
private:
	struct {
		const uintptr_t draw_ln = 0x407A83;
		const uintptr_t cmp = 0x4135AB;
	} m_offsets;

	static void OnDrawLN(safetyhook::Context& ctx);
	static void OnSliderCmp(safetyhook::Context& ctx);

	SafetyHookMid m_draw_ln_hook;
	SafetyHookMid m_slider_hook;
public:
	int m_lift_number_p1; /* these must exist since SliderByTime() requires a pointer to the value instead of just the value */
	int m_lift_number_p2;

	SkinMisc();
};
