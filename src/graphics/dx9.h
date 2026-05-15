#pragma once
#include <safetyhook.hpp>
#include "gui.h"

namespace dx9
{
    inline SafetyHookInline present_hook = {};
    inline SafetyHookInline end_scene_hook = {};
    inline SafetyHookInline reset_hook = {};
    inline SafetyHookInline resetEx_hook = {};

    HRESULT __stdcall hook_present(LPDIRECT3DSWAPCHAIN9 pSwapchain, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags) noexcept;
    HRESULT __stdcall hook_end_scene(IDirect3DDevice9* device) noexcept;
    HRESULT __stdcall hook_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
    HRESULT __stdcall hook_resetEx(IDirect3DDevice9Ex* device, D3DPRESENT_PARAMETERS* params, D3DDISPLAYMODEEX* display);

    constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
    {
        return (*static_cast<void***>(thisptr))[index];
    }

    void Setup();
    void Destroy() noexcept;
}