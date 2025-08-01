#pragma once
#include <safetyhook.hpp>
#include "utils/statistics.h"
#include "features/greennumber.h"
#include "features/timeplayed.h"
#include "features/totaltime.h"

/*namespace hooks::srcnumber {
    namespace offsets {
        const inline uintptr_t src_number = 0x4024D0;
        const inline uintptr_t random_address = 0x04B43F1;
              inline uintptr_t random = 0xDC35C; // not const because fuck windows
        const inline uintptr_t game_offset = 0xFF838;
        const inline uintptr_t hp_offset = 0x979C8;
    }

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
}*/

class SrcNumber {
private:
    struct {
        const uintptr_t src_number = 0x4024D0;
        const uintptr_t random_address = 0x04B43F1;
    } m_offsets;

    static int OnSrcNumber(uintptr_t* data_ptr, int id);
    
    static int GetWhole(double num);
    static int GetDecimal(double num, size_t number_of_places);
    static double GetRatio(double lhs, double rhs);
    static double GetPercentage(double lhs, double rhs);
public:
    SrcNumber();
    void Reset();
    
    statistics::OnlineMean m_mean = statistics::OnlineMean();
    statistics::OnlineStandardDeviation m_stddev = statistics::OnlineStandardDeviation();

    Timer m_timer;
    GreenNumber m_green_number;

    SafetyHookInline m_src_number_hook;
    SafetyHookMid m_random_address_hook;
};