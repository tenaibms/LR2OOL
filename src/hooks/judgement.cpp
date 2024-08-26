#include "judgement.h"
#include "features/hiterror.h"
#include "hooks/srcnumber.h"

int __cdecl hooks::judgement::OnJudgement(int a1, int a2, int a3, int a4, int a5, char a6)
{
    // todo: clean this up
    int result = judge_hook.ccall<int>(a1, a2, a3, a4, a5, a6);
    JUDGEMENT judgement = static_cast<JUDGEMENT>(a1);

    /* we don't handle miss poors */
    if (judgement == JUDGEMENT::miss_poor)
        return result;

    int hit_timing = *reinterpret_cast<int*>(0x127300);
    int note_timing = *reinterpret_cast<int*>(0x127304);
    /* order is important here, as this makes early hits negative, as is convention */
    int judgement_delta = (hit_timing - note_timing);

    /* empty poors should not affect timing metrics */
    if (judgement != JUDGEMENT::empty_poor) {
        hiterror::ema.Insert(judgement_delta);
        srcnumber::mean.Insert(judgement_delta);
        srcnumber::stddev.Insert(judgement_delta);
    }

    /* they should still be shown in the hit error bar however */
    hiterror::InsertBuffer(std::clamp(judgement_delta, -255, 255), judgement);

    return result;
}


void hooks::judgement::Install()
{
	judge_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::update_judge_data), reinterpret_cast<void*>(OnJudgement));
}
