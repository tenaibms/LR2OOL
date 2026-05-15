#include <d3d9.h>

#include <safetyhook.hpp>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "dx9.h"

struct SceneDesc {
    struct int2 {
        int x = 0;
        int y = 0;

        auto operator<=>(const int2&) const = default;
    };
    int2 canvasSize;
    int2 outputSize;
    SceneDesc(int canvasX, int canvasY, int outputX, int outputY) :
        canvasSize(canvasX, canvasY), outputSize(outputX, outputY) {
    };
};

static int sceneIdx = -1;
static int sceneTarget = -1;
static std::vector<SceneDesc> scenes;
HRESULT __stdcall dx9::hook_present(LPDIRECT3DSWAPCHAIN9 pSwapchain, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags) noexcept
{
    if (!scenes.empty()) {
        int target = scenes.size() - 1;
        if (scenes.size() > 1) {
            if (scenes[target].canvasSize != scenes[target - 1].canvasSize) target--;
        }
        sceneTarget = target;
        scenes.clear();
    }
    sceneIdx = -1;
    return present_hook.stdcall<HRESULT>(pSwapchain, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

HRESULT __stdcall dx9::hook_end_scene(IDirect3DDevice9* device) noexcept
{
    if (!gui::setup) {
        gui::SetupMenu(device);
    }

    gui::Reset(device);

    D3DSURFACE_DESC canvas{};
    {
        IDirect3DSurface9* rt{};
        device->GetRenderTarget(0, &rt);
        rt->GetDesc(&canvas);
        rt->Release();
    }
    D3DSURFACE_DESC output{};
    {
        IDirect3DSurface9* backbuffer{};
        device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
        backbuffer->GetDesc(&output);
        backbuffer->Release();
    }
    scenes.emplace_back(canvas.Width, canvas.Height, output.Width, output.Height);

    sceneIdx++;
    if (sceneIdx != sceneTarget) return end_scene_hook.stdcall<HRESULT>(device);

    if (IsIconic(gui::window)) return end_scene_hook.stdcall<HRESULT>(device);

    gui::internal_resolution[0] = canvas.Width;
    gui::internal_resolution[1] = canvas.Height;
    gui::output_resolution[0] = output.Width;
    gui::output_resolution[1] = output.Height;
    gui::Render();

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

HRESULT __stdcall dx9::hook_resetEx(IDirect3DDevice9Ex* device, D3DPRESENT_PARAMETERS* params, D3DDISPLAYMODEEX* display)
{
    if (device == NULL) {
        return resetEx_hook.stdcall<HRESULT>(device, params, display);
    }

    ImGui_ImplDX9_InvalidateDeviceObjects();
    const auto result = resetEx_hook.stdcall<HRESULT>(device, params, display);
    if (SUCCEEDED(result)) ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}

void dx9::Setup()
{
    present_hook = safetyhook::create_inline(VirtualFunction(gui::dummySwapchain, 3), hook_present);
    end_scene_hook = safetyhook::create_inline(VirtualFunction(gui::dummyDevice, 42), hook_end_scene);
    reset_hook = safetyhook::create_inline(VirtualFunction(gui::dummyDevice, 16), hook_reset);
    resetEx_hook = safetyhook::create_inline(VirtualFunction(gui::dummyDevice, 132), hook_resetEx);
}

void dx9::Destroy() noexcept
{
    present_hook = {};
    end_scene_hook = {};
    reset_hook = {};
    resetEx_hook = {};
}
