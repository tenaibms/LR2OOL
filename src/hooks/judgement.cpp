#include "judgement.h"
#include "features/hiterror.h"
#include "hooks/srcnumber.h"

JudgementProcessing::JudgementProcessing()
{
    process_note_single_hook = safetyhook::create_inline((void*)(m_offsets.process_note_single), OnProcessNoteSingle);

    m_mid_hooks.push_back(safetyhook::create_mid((void*)m_offsets.store_note_time, OnStoreNoteTime));
    
    /* single note hooks */
    m_mid_hooks.push_back(safetyhook::create_mid((void*)m_offsets.judge_to_score_single.pgreat, OnCallJudgeToScore));
    m_mid_hooks.push_back(safetyhook::create_mid((void*)m_offsets.judge_to_score_single.great, OnCallJudgeToScore));
    m_mid_hooks.push_back(safetyhook::create_mid((void*)m_offsets.judge_to_score_single.good, OnCallJudgeToScore));
    m_mid_hooks.push_back(safetyhook::create_mid((void*)m_offsets.judge_to_score_single.bad, OnCallJudgeToScore));
    m_mid_hooks.push_back(safetyhook::create_mid((void*)m_offsets.judge_to_score_single.empty_poor, OnCallJudgeToScore));

    /* long note hooks */

}

int __cdecl JudgementProcessing::OnProcessNoteSingle(void* g, int lane, int keypress, int timing, int player)
{
    hooks::judgement_processing.current_time = timing;
    return hooks::judgement_processing.process_note_single_hook.ccall<int>(g, lane, keypress, timing, player);
}

void JudgementProcessing::OnCallJudgeToScore(SafetyHookContext& ctx)
{
    Judgement judgement = *(Judgement*)(ctx.esp);
    int judgement_delta = hooks::judgement_processing.current_time - hooks::judgement_processing.note_time;

    if (judgement != Judgement::EMPTY_POOR) {
        hiterror::ema.Insert(judgement_delta);
        hooks::srcnumber::mean.Insert(judgement_delta);
        hooks::srcnumber::stddev.Insert(judgement_delta);
    }

    hiterror::InsertBuffer(judgement_delta, judgement);
}

void JudgementProcessing::OnStoreNoteTime(SafetyHookContext& ctx)
{
    hooks::judgement_processing.note_time = ctx.eax;
}
