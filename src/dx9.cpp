#include "dx9.h"
#include "hooks.h"
#include <d3d9.h>
#include <stdexcept>
#include <intrin.h>
#include <safetyhook.hpp>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

HRESULT __stdcall dx9::hook_end_scene(IDirect3DDevice9* device) noexcept
{
    static const auto return_address = _ReturnAddress();
    const auto result = end_scene_hook.stdcall<HRESULT>(device);
    
    // not necessary for LR2 maybe...?
    /*
    if (_ReturnAddress() == return_address)
        return result;
    */
    

    if (!gui::setup)
        gui::SetupMenu(device);
    /* cursor stuff, do later
    if (gui::open) {

    }
    else {
        p[0] = 0x00;
        p[7] = 0x83;
        p[8] = 0xF8;
        p[9] = 0xFF;
        p[10] = 0x7E;
        p[11] = 0x02;
        p[12] = 0xEB;
        p[13] = 0xF1;
    }*/

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
