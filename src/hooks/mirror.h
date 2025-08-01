#pragma once
#include <safetyhook.hpp>

class Mirror {
public:
    Mirror(); // Constructor installs hooks

    bool m_enabled = false;

private:
    struct {
        static constexpr uintptr_t mirror = 0x433A5A;
        static constexpr uintptr_t reset = 0x433A61;
        static constexpr uintptr_t current_opt = 0x0FF848;
    } m_offsets;

    SafetyHookMid m_mirror_hook;
    SafetyHookMid m_reset_hook;
};