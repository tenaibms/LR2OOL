#pragma once
#include <safetyhook.hpp>
#include "gui.h"

namespace dx9
{
    inline SafetyHookInline end_scene_hook = {};
    inline SafetyHookInline reset_hook = {};

    HRESULT __stdcall hook_end_scene(IDirect3DDevice9* device) noexcept;
    HRESULT __stdcall hook_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);

    constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
    {
        return (*static_cast<void***>(thisptr))[index];
    }

    void Setup();
    void Destroy() noexcept;
}