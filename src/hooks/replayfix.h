#pragma once
#include <safetyhook.hpp>

class ReplayFix {
public:
	ReplayFix();

	bool m_enabled = false;

	SafetyHookInline save_replay_hook;
private:
	struct {
		const uintptr_t save_replay = 0x4C09E0;
		const uintptr_t replay_guage = 0x50;
	} m_offsets;
	
	static int __cdecl OnSaveReplay(uint32_t* replay, void* song_md5, void* ArgList);
};
