#pragma once
#include <safetyhook.hpp>

enum class JUDGEMENT {
	empty_poor,
	miss_poor,
	bad,
	good,
	great,
	pgreat
};

namespace hooks::judgement {
	namespace offsets {
		const inline uintptr_t update_judge_data = 0x405FB0;
	}

	inline SafetyHookInline judge_hook;
	int __cdecl OnJudgement(int judgement, int a2, int a3, int a4, int a5, char a6);

	void Install();
}