#pragma once
#include <safetyhook.hpp>

enum class Judgement {
	EMPTY_POOR,
	MISS_POOR,
	BAD,
	GOOD,
	GREAT,
	PGREAT
};

class JudgementProcessing {
public:
	JudgementProcessing();
private:
	struct {
		const uintptr_t process_note_single = 0x418850;
		const uintptr_t store_note_time = 0x418A18;
		struct {
			const uintptr_t pgreat = 0x418A7A;
			const uintptr_t great = 0x418B3A;
			const uintptr_t good = 0x418BFA;
			const uintptr_t bad = 0x418CA8;
			const uintptr_t empty_poor = 0x418D91;
		} judge_to_score_single;
	} m_offsets;
	
	int note_time = 0;
	int current_time = 0;

	std::vector<SafetyHookMid> m_mid_hooks;
	SafetyHookInline process_note_single_hook;

	static int __cdecl OnProcessNoteSingle(void* g, int lane, int keypress, int timing, int player);
	static void OnCallJudgeToScore(SafetyHookContext& ctx);
	static void OnStoreNoteTime(SafetyHookContext& ctx);
};