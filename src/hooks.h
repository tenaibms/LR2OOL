#pragma once
#include <safetyhook.hpp>
#include <vector>
#include <stdint.h>
#include "statistics.h"
#include <Windows.h>

// TODO: clean this up

namespace offsets {
    const inline uintptr_t src_number = 0x4024D0;
    const inline uintptr_t random = 0xDC35C;
    const inline uintptr_t current_opt = 0x0FF848;
    const inline uintptr_t mirror = 0x433A5A;
    const inline uintptr_t update_judge_data = 0x405FB0;
    const inline uintptr_t change_gamestate = 0x431BB9;
    const inline uintptr_t cursor_patch = 0x4D09E0;
    const inline uintptr_t internal_resolution = 0x7A3B50;
    const inline uintptr_t pgreat_window = 0x197274;
    const inline uintptr_t great_window = 0x197270;
    const inline uintptr_t good_window = 0x19726C;
    const inline uintptr_t bad_window = 0x197264;
    const inline uintptr_t save_replay = 0x4C09E0;
    const inline uintptr_t current_guage = 0x00FF840;
    //const inline uintptr_t wind_proc = 0x4CB5C0;
}

namespace hooks {
    // cursor hook
    inline SafetyHookInline cursor_hook;
    int __cdecl hook_cursor(int enabled);

    // mirror hook
    inline bool mirror_hook_enabled;
    inline SafetyHookMid mirror_hook;
    inline SafetyHookMid reset_hook;

    // playing hook
    inline SafetyHookMid gamestate_hook;

    // judge hook
    inline SafetyHookInline judge_hook;
    int __cdecl hook_judge(int judgement, int a2, int a3, int a4, int a5, char a6);

    // src number hook
    inline SafetyHookInline src_number_hook;
    int hook_src_number(uintptr_t* data_ptr, int id);

    // replay save fix hook hook
    inline bool save_replay_hook_enabled = false;
    inline SafetyHookInline save_replay_hook;
    int __cdecl hook_save_replay(uint32_t* replay, int a2, void* ArgList);

    // wind_proc hook
    inline SafetyHookInline wind_proc_hook;
    LRESULT __stdcall hook_wind_proc(HWND hWnd, UINT Msg, __int64 wParam);

    void Setup();
    void Destroy();
}

namespace src_numbers {

    inline statistics::OnlineMean mean = statistics::OnlineMean();
    inline statistics::OnlineStandardDeviation stddev = statistics::OnlineStandardDeviation();
}