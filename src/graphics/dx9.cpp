#include <d3d9.h>

#include <safetyhook.hpp>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "dx9.h"


HRESULT __stdcall dx9::hook_end_scene(IDirect3DDevice9* device) noexcept
{    
    if (!gui::setup) {
        gui::SetupMenu(device);
    }

    if (gui::device != device) {
        gui::Reset(device);
    }

    IDirect3DSurface9* backbuffer;
    if (SUCCEEDED(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer))) {
        gui::Render();
        backbuffer->Release();
    }

    return end_scene_hook.stdcall<HRESULT>(device);;
}

HRESULT __stdcall dx9::hook_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
    if (device == NULL) {
        return reset_hook.stdcall<HRESULT>(device, params);
    }

    ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = reset_hook.stdcall<HRESULT>(device, params);
    if(SUCCEEDED(result)) ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}

void dx9::Setup()
{
    end_scene_hook = safetyhook::create_inline(VirtualFunction(gui::dummyDevice, 42), reinterpret_cast<void*>(hook_end_scene));
    reset_hook = safetyhook::create_inline(VirtualFunction(gui::dummyDevice, 16), reinterpret_cast<void*>(hook_reset));
}

void dx9::Destroy() noexcept
{
    end_scene_hook = {};
    reset_hook = {};
}
