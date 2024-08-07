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

int hooks::hook_src_number(uintptr_t* data_ptr, uint32_t id)
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
        return (int)src_numbers::mean.GetMean();
        break;
    case 297: // decimal part of mean
        return abs((int)100*(src_numbers::mean.GetMean() - (int) src_numbers::mean.GetMean()));
        break;
    case 298: // whole part of stddev
        return (int)src_numbers::stddev.GetPopulationStandardDeviation();
        break;
    case 299: // decimal part of stddev
        return abs((int)100 * (src_numbers::stddev.GetPopulationStandardDeviation() - (int)src_numbers::stddev.GetPopulationStandardDeviation()));
        break;
    default:
        return src_number_hook.call<int>(data_ptr, id);
        break;
    }
    return 0;
}

void hooks::Setup()
{
    judge_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::update_judge_data), reinterpret_cast<void*>(hook_judge));

    gamestate_hook = safetyhook::create_mid(reinterpret_cast<void*>(offsets::change_gamestate), [](safetyhook::Context& ctx) {
        if (ctx.eax == 2) { // todo: convert this game state into an enum and create helper functions instead of setting these variables directly
            hiterror::open = true;
            hiterror::buffer_current = 0;
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
}

void hooks::Destroy()
{
}