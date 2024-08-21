#include <vector>

#include <numeric>

#include "hooks.h"
#include "gui.h"
#include "hiterror.h"
#include "statistics.h"

int __cdecl hooks::hook_cursor(int enabled)
{
    if(gui::open)
        return cursor_hook.call<int>(1);
    return cursor_hook.call<int>(enabled);
}

int __cdecl hooks::hook_judge(int a1, int a2, int a3, int a4, int a5, char a6)
{
    // todo: clean this up
    int result = judge_hook.ccall<int>(a1, a2, a3, a4, a5, a6);
    JUDGEMENT judgement = static_cast<JUDGEMENT>(a1);
    if (judgement == JUDGEMENT::miss_poor)
        return result; // don't handle miss poors
    
    int hit_timing = *reinterpret_cast<int*>(0x127300);
    int note_timing = *reinterpret_cast<int*>(0x127304);
    int judgement_delta = (hit_timing - note_timing); // order matters here, early hits should be negative
    
    // we don't want to clutter our timing metrics with empty poors as they can significantly skew the data
    if(judgement != JUDGEMENT::empty_poor) {
        hiterror::UpdateEma(judgement_delta);
        src_numbers::mean.Insert(judgement_delta);
        src_numbers::stddev.Insert(judgement_delta);
    }

    // however we still want them to show up on our graph
    hiterror::InsertBuffer(std::clamp(judgement_delta, -255, 255), static_cast<JUDGEMENT>(a1));

    return result;
}

int GetWhole(double num)
{
    return (int) num;
}
int GetDecimal(double num)
{
    return abs((int)100*(num - (int)num));
}

int hooks::hook_src_number(uintptr_t* data_ptr, int id)
{
    switch (id) {
    case 295: // current random
    {
        uint32_t current_random = 0;

        int* note_positions = reinterpret_cast<int*>(offsets::random);
        for (int i = 0; i < 7; i++) {
            current_random += (i + 1) * pow(10, 7 - note_positions[i]);
        }

        return current_random;
        break;
    }
    case 296: // whole part of mean
        return GetWhole(src_numbers::mean.GetMean());
        break;
    case 297: // decimal part of mean
        return GetDecimal(src_numbers::mean.GetMean());
        break;
    case 298: // whole part of stddev
        return GetWhole(src_numbers::stddev.GetPopulationStandardDeviation());
        break;
    case 299: // decimal part of stddev
        return GetDecimal(src_numbers::stddev.GetPopulationStandardDeviation());
        break;
    case 400: // whole part of pgreat ratio
        return GetWhole((double)hooks::src_number_hook.call<int>(data_ptr, 110) / (double)hooks::src_number_hook.call<int>(data_ptr, 111));
        break;
    case 401: // decimal part of pgreat ratio
        return GetDecimal((double)hooks::src_number_hook.call<int>(data_ptr, 110) / (double)hooks::src_number_hook.call<int>(data_ptr, 111));
        break;
    case 402: // whole part of great ratio
        return GetWhole((double)hooks::src_number_hook.call<int>(data_ptr, 111) / (double)hooks::src_number_hook.call<int>(data_ptr, 112));
        break;
    case 403: // decimal part of great ratio
        return GetDecimal((double)hooks::src_number_hook.call<int>(data_ptr, 111) / (double)hooks::src_number_hook.call<int>(data_ptr, 112));
        break;
    default:
        return src_number_hook.call<int>(data_ptr, id);
        break;
    }
    return 0;
}

int __cdecl hooks::hook_save_replay(uint32_t* replay, int a2, void* ArgList)
{
    if(save_replay_hook_enabled) {
        uint8_t* replay_gauge = ((uint8_t*)(*replay + 0x50));
        uint8_t current_gauge = *((uint8_t*)(offsets::current_guage));
        if (*replay_gauge != current_gauge) {
            *replay_gauge = current_gauge;
        }
    }
    return save_replay_hook.ccall<int>(replay, a2, ArgList);
}

/*LRESULT __stdcall hooks::hook_wind_proc(HWND hWnd, UINT msg, __int64 wParam)
{
    ImGuiIO &io = ImGui::GetIO();
    if (gui::open && (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP))
        return 1L;
    if (gui::open && (msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_CHAR || msg == WM_UNICHAR || msg == WM_SYSKEYDOWN))
        return 1L;
    return wind_proc_hook.stdcall<LRESULT>(hWnd, msg, wParam);
}*/

void hooks::Setup()
{
    judge_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::update_judge_data), reinterpret_cast<void*>(hook_judge));

    gamestate_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::change_gamestate), [](safetyhook::Context& ctx) {
        if (ctx.eax == 2) { // todo: convert this game state into an enum and create helper functions instead of setting these variables directly
            hiterror::open = true;
            hiterror::buffer_current = 0;
            hiterror::ema = 0;
            src_numbers::mean = statistics::OnlineMean();
            src_numbers::stddev = statistics::OnlineStandardDeviation();
        } else {
            hiterror::open = false;
        }
    });

    cursor_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::cursor_patch), reinterpret_cast<void*>(hooks::hook_cursor));

    mirror_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::mirror), [](safetyhook::Context& ctx) {
        if (hooks::mirror_hook_enabled) {
            int current_opt = *reinterpret_cast<int*>(offsets::current_opt);
            current_opt == 1 ? ctx.ebx = current_opt : ctx.ebx = 0;
        }
    });
    reset_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::mirror + 7), [](safetyhook::Context& ctx) {
        ctx.ebx = 0;
    });

    src_number_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::src_number), reinterpret_cast<void*>(hook_src_number));

    save_replay_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::save_replay), reinterpret_cast<void*>(hook_save_replay));

    //wind_proc_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::wind_proc), reinterpret_cast<void*>(hook_wind_proc));
}

void hooks::Destroy()
{
#define O(x) x = {}
    O(judge_hook);
    O(gamestate_hook);
    O(cursor_hook);
    O(mirror_hook);
    O(reset_hook);
    O(src_number_hook);
    O(save_replay_hook);
#undef O
}