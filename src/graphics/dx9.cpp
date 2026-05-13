#include <d3d9.h>

#include <safetyhook.hpp>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "dx9.h"

static int sceneIdx = 0;
HRESULT __stdcall dx9::hook_present(LPDIRECT3DSWAPCHAIN9 pSwapchain, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags) noexcept
{
    sceneIdx = 0;
    return present_hook.stdcall<HRESULT>(pSwapchain, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

HRESULT __stdcall dx9::hook_end_scene(IDirect3DDevice9* device) noexcept
{
    int curSceneIdx = sceneIdx;
    sceneIdx++;
    if (curSceneIdx != 0) return end_scene_hook.stdcall<HRESULT>(device);

    if (!gui::setup) {
        gui::SetupMenu(device);
    }

    gui::Reset(device);

    if (IsIconic(gui::window)) return end_scene_hook.stdcall<HRESULT>(device);

    std::vector<IDirect3DSurface9*> rts{};
    rts.reserve(gui::rtMax);
    for (int i = 0; i < rts.capacity(); i++) {
        IDirect3DSurface9* rt{};
        device->GetRenderTarget(i, &rt);
        rts.push_back(rt);
        if (i != 0) device->SetRenderTarget(i, NULL);
    }
    D3DSURFACE_DESC canvas{};
    rts[0]->GetDesc(&canvas);

    IDirect3DSurface9* backbuffer{};
    D3DSURFACE_DESC output{};
    device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
    backbuffer->GetDesc(&output);
    backbuffer->Release();

    gui::internal_resolution[0] = canvas.Width;
    gui::internal_resolution[1] = canvas.Height;
    gui::output_resolution[0] = output.Width;
    gui::output_resolution[1] = output.Height;
    gui::Render();

    for (int i = 0; i < rts.size(); i++) {
        auto& rt = rts[i];
        device->SetRenderTarget(i, rt);
        if (rt) rt->Release();
    }

    return end_scene_hook.stdcall<HRESULT>(device);
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
    //reset_hook = {};
}
