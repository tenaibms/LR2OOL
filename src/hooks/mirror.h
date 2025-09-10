#pragma once
#include <safetyhook.hpp>

class Mirror {
public:
    Mirror();

    bool m_enabled = false;

private:
    struct {
        const uintptr_t random_p1 = 0x433A5A;
        const uintptr_t random_p2 = 0x433A61;
        const uintptr_t reset_1 = 0x433A68;
        const uintptr_t dp_flip = 0x433B09;
        const uintptr_t reset_2 = 0x433B10;
    } m_offsets;

    static bool IsSPMirror();
    static bool IsDPMirror();

    SafetyHookMid m_random_p1_hook;
    SafetyHookMid m_random_p2_hook;
    SafetyHookMid reset_1_hook;
    SafetyHookMid dp_flip_hook;
    SafetyHookMid reset_2_hook;
};