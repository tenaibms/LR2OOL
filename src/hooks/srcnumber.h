#pragma once
#include <safetyhook.hpp>
#include "utils/statistics.h"
#include "features/greennumber.h"
#include "features/timeplayed.h"
#include "features/totaltime.h"

namespace hooks::srcnumber {
    namespace offsets {
        const inline uintptr_t src_number = 0x4024D0;
        const inline uintptr_t random_address = 0x04B43F1;
              inline uintptr_t random = 0xDC35C; // not const because fuck windows
        const inline uintptr_t game_offset = 0xFF838;
        const inline uintptr_t hp_offset = 0x979C8;
    }

    /* data pointers */
    inline statistics::OnlineMean mean = statistics::OnlineMean();
    inline statistics::OnlineStandardDeviation stddev = statistics::OnlineStandardDeviation();

    inline Timer timer;
    inline GreenNumber green_number;

    inline SafetyHookInline src_number_hook;
    inline SafetyHookMid random_address_hook;

    int SrcNumber(uintptr_t* data_ptr, int id);

    inline int GetWhole(double num);
    inline int GetDecimal(double num, size_t number_of_places);
    
    void Install();
    void Reset();
}