#pragma once
#include <stdint.h>
#include <Windows.h>
#include <d3d9.h>
#include <imgui_internal.h>

namespace gui {
    // is menu ready to be shown?
    inline bool setup = false;

    // winapi stuff
    inline HWND window = nullptr;
    inline WNDCLASSEX window_class = {0};
    inline WNDPROC original_window_process = nullptr;

    // directX stuff
    inline LPDIRECT3DDEVICE9 device = nullptr;
    inline LPDIRECT3D9 d3d9 = nullptr;

    bool SetupWindowClass(const char* window_class_name) noexcept;
    void DestroyWindowClass() noexcept;

    bool SetupWindow(const char* window_name) noexcept;
    void DestroyWindow() noexcept;

    bool SetupDirectX() noexcept;
    void DestroyDirectX() noexcept;

    void Setup();

    void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
    void Destroy();

    void Render();
}