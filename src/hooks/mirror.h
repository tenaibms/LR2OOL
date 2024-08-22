#pragma once
#include <safetyhook.hpp>

namespace hooks::mirror {
    namespace offsets {
        const inline uintptr_t mirror = 0x433A5A;
        const inline uintptr_t reset = 0x433A61;
        const inline uintptr_t current_opt = 0x0FF848;
    }

    /* data pointers */
    inline int current_opt = *reinterpret_cast<int*>(offsets::current_opt);

    inline bool enabled = false;
    inline SafetyHookMid mirror_hook;
    inline SafetyHookMid reset_hook;

    void Install();
}