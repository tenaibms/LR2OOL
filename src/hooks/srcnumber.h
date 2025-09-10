#pragma once
#include <safetyhook.hpp>
#include "utils/statistics.h"
#include "features/greennumber.h"
#include "features/timeplayed.h"
#include "features/totaltime.h"

class SrcNumber {
public:
    SrcNumber() = default;
    void Init();
    void Reset();

    statistics::OnlineMean m_mean = statistics::OnlineMean();
    statistics::OnlineStandardDeviation m_stddev = statistics::OnlineStandardDeviation();

    Timer m_timer;
    GreenNumber m_green_number;

    SafetyHookInline m_src_number_hook;
    SafetyHookMid m_random_address_hook;
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
};