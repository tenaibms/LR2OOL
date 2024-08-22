#include <d3d9.h>

#include <safetyhook.hpp>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "dx9.h"


HRESULT __stdcall dx9::hook_end_scene(IDirect3DDevice9* device) noexcept
{
    const auto result = end_scene_hook.stdcall<HRESULT>(device);
    
    if (!gui::setup)
        gui::SetupMenu(device);

    gui::Render();

    return result;
}

HRESULT __stdcall dx9::hook_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = reset_hook.stdcall<HRESULT>(device, params);
    ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}

void dx9::Setup()
{
    end_scene_hook = safetyhook::create_inline(VirtualFunction(gui::device, 42), reinterpret_cast<void*>(hook_end_scene));
    reset_hook = safetyhook::create_inline(VirtualFunction(gui::device, 16), reinterpret_cast<void*>(hook_reset));
}

void dx9::Destroy() noexcept
{
    end_scene_hook = {};
    reset_hook = {};
}
