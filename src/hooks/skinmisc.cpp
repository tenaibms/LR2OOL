#include "skinmisc.h"
#include "hooks/hooks.h"
#include "features/greennumber.h"
#include "hooks/updategamestate.h"
#include <LR2Bindings.hpp>
#include <LR2Typedefs.hpp>

using slider_by_time_type = int(*)(LR2::DrawingBuf*, LR2::SRCstruct*, LR2::DSTstruct*, LR2::Timer*, int, int, int*, LR2::inputStructure*, int);
slider_by_time_type SliderByTime = (slider_by_time_type)0x49C0E0;

void SkinMisc::OnDrawLN(safetyhook::Context& ctx)
{
	LR2::SkinAdjust* adjust = &LR2::pGame->skstruct.adjust;
	*(float*)(ctx.esp + 0x20) += ctx.esi < 10 ? adjust->note_1p_y : adjust->note_2p_y;
}

void SkinMisc::OnSliderCmp(safetyhook::Context& ctx)
{
	int i = ctx.ebx;
	int case_num = ctx.ecx + 1;
	LR2::skstruct* sk = &LR2::pGame->skstruct;

	if (hooks::updategamestate::gamestate == hooks::updategamestate::GAMESTATE::playing) {
		hooks::skin_misc.m_lift_number_p1 = hooks::src_number.m_green_number.GetLiftNumber(1);
		hooks::skin_misc.m_lift_number_p2 = hooks::src_number.m_green_number.GetLiftNumber(2);
	}

	if (case_num == 27) {
		SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], (LR2::Timer*)ctx.edi, 0, 1000, &hooks::skin_misc.m_lift_number_p1, &LR2::pGame->KeyInput, i);
	}
	else if (case_num == 28) {
		SliderByTime(&sk->drBuf, &sk->otherObject[2].src[i], &sk->otherObject[2].dst[i], (LR2::Timer*)ctx.edi, 0, 1000, &hooks::skin_misc.m_lift_number_p2, &LR2::pGame->KeyInput, i);
	}
}

int SkinMisc::OnDrawNum(LR2::DrawingBuf* drb, LR2::SRCstruct* src, LR2::DSTstruct* dst, LR2::Timer* T, int number, int x, int y)
{
	if (src->op1 == 210 || src->op1 == 211 && hooks::skin_misc.m_fs) {
		y = LR2::pGame->skstruct.adjust.judge_y;
	}
	if (src->op1 == 108 && LR2::pGame->config.play.play_ghost == 1 && hooks::skin_misc.m_pacemaker) {
		y = LR2::pGame->skstruct.adjust.judge_y;
	}

	return hooks::skin_misc.m_draw_num_hook.call<int>(drb, src, dst, T, number, x, y);
}

SkinMisc::SkinMisc()
{
	m_draw_ln_hook = safetyhook::create_mid(m_offsets.draw_ln, OnDrawLN);
	m_slider_hook = safetyhook::create_mid(m_offsets.cmp, OnSliderCmp);
	m_draw_num_hook = safetyhook::create_inline((void*)m_offsets.draw_num, OnDrawNum);
}
