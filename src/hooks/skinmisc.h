#pragma once
#include <safetyhook.hpp>
#include <LR2Typedefs.hpp>

class SkinMisc {
public:
	SkinMisc() = default;
	void Init();

	int m_lift_number_p1 = 0; /* these must exist since SliderByTime() requires a pointer to the value instead of just the value */
	int m_lift_number_p2 = 0;

	bool m_fs = false;
	bool m_pacemaker = false;
	bool m_judge = false;

private:
	struct {
		const uintptr_t draw_ln = 0x407A83;
		const uintptr_t cmp = 0x4135AB;
		const uintptr_t draw_num = 0x49E060;
		const uintptr_t draw_judge = 0x49E550;
	} m_offsets;

	static void OnDrawLN(safetyhook::Context& ctx);
	static void OnSliderCmp(safetyhook::Context& ctx);
	static int OnDrawNum(LR2::DrawingBuf* drb, LR2::SRCstruct* src, LR2::DSTstruct* dst, LR2::Timer* T, int number, int x, int y);
	static int OnDrawJudge(LR2::DrawingBuf* drb, LR2::SRCstruct* jsrc, LR2::DSTstruct* jdst, LR2::SRCstruct* csrc, LR2::DSTstruct* cdst, LR2::Timer* T, int combo, int x, int y);

	SafetyHookMid m_draw_ln_hook;
	SafetyHookMid m_slider_hook;
	SafetyHookInline m_draw_num_hook;
	SafetyHookInline m_draw_judge_hook;
};
