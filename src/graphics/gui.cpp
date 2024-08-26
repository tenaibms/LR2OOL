#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <stdexcept>

#include <windowsx.h>

#include "gui.h"
#include "features/hiterror.h"
#include "overlay/imguistyle.h"
#include "overlay/overlay.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool gui::SetupWindowClass(const char* window_class_name) noexcept
{
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = DefWindowProc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = GetModuleHandle(NULL);
    window_class.hIcon = NULL;
    window_class.hCursor = NULL;
    window_class.hbrBackground = NULL;
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = window_class_name;
    window_class.hIconSm = NULL;

    if (!RegisterClassEx(&window_class))
    {
        return false;
    }

    return true;
}

void gui::DestroyWindowClass() noexcept
{
    UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}

bool gui::SetupWindow(const char* window_name) noexcept
{
    window = CreateWindow(
        window_class.lpszClassName,
        window_name,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        100,
        100,
        0,
        0,
        window_class.hInstance,
        0
    );

    if (!window)
        return false;

    return true;
}

void gui::DestroyWindow() noexcept
{
    if (window)
    {
        DestroyWindow(window);
    }
}

bool gui::SetupDirectX() noexcept
{
    HMODULE handle = nullptr;
    do {
        handle = GetModuleHandle("d3d9.dll");
        Sleep(10);
    } while (!handle);

    using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
    CreateFn create = nullptr;
    do { 
        create = reinterpret_cast<CreateFn>(GetProcAddress(
        handle,
        "Direct3DCreate9"
        ));
        Sleep(10);
    } while (!create);

    d3d9 = create(D3D_SDK_VERSION);

    if (!d3d9)
        return false;

    D3DPRESENT_PARAMETERS params = { };
    params.BackBufferWidth = 0;
    params.BackBufferHeight = 0;
    params.BackBufferFormat = D3DFMT_UNKNOWN;
    params.BackBufferCount = 0;
    params.MultiSampleType = D3DMULTISAMPLE_NONE;
    params.MultiSampleQuality = NULL;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = window;
    params.Windowed = 1;
    params.EnableAutoDepthStencil = 0;
    params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
    params.Flags = NULL;
    params.FullScreen_RefreshRateInHz = 0;
    params.PresentationInterval = 0;

    if (d3d9->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_NULLREF,
        window,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
        &params,
        &device
    ) < 0)
        return false;

    return true;
}

void gui::DestroyDirectX() noexcept
{
    if (device)
    {
        device->Release();
        device = NULL;
    }

    if (d3d9)
    {
        d3d9->Release();
        d3d9 = NULL;
    }
}

void gui::Setup()
{
    if (!SetupWindowClass("Hack Class"))
    {
        throw std::runtime_error("Failed to create window class.");
    }

    if (!SetupWindow("Hack Window"))
    {
        throw std::runtime_error("Failed to create window.");
    }

    if (!SetupDirectX())
    {
        throw std::runtime_error("Failed to create device.");
    }

    DestroyWindow();
    DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
    auto params = D3DDEVICE_CREATION_PARAMETERS{ };
    device->GetCreationParameters(&params);

    window = params.hFocusWindow;

    original_window_process = reinterpret_cast<WNDPROC>(
        SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
    );

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    hiterror::ClearBuffer();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

    SetStyleGrey();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1.f, 1.f));

    setup = true;
}

void gui::Destroy()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    SetWindowLongPtr(
        window,
        GWLP_WNDPROC,
        reinterpret_cast<LONG_PTR>(original_window_process)
    );

    DestroyDirectX();
}

void gui::Render()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    overlay::Render();
    hiterror::Render();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    if (GetAsyncKeyState(VK_INSERT) & 1)
        overlay::open = !overlay::open;

    LPARAM imgui_lParam = lParam;

    // this fixes mouse coordinates not scaling with the window when using a non-native resolution for lr2/lr2hd

    if (msg == WM_MOUSEMOVE || msg == WM_NCMOUSEMOVE) {
        RECT r;
        GetClientRect(hWnd, &r); // maybe should only update this on resize..? seems wasteful to do this every time a message is called
        int physical_resolution[2] = { r.right - r.left, r.bottom - r.top };

        float scaling_factor_x = (float)gui::internal_resolution[0] / physical_resolution[0];
        float scaling_factor_y = (float)gui::internal_resolution[1] / physical_resolution[1];

        POINT mouse_pos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        mouse_pos.x *= scaling_factor_x;
        mouse_pos.y *= scaling_factor_y;
        imgui_lParam = MAKELPARAM(mouse_pos.x, mouse_pos.y);
    }

    if (overlay::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, imgui_lParam))
        return 1L;

    return CallWindowProc(gui::original_window_process, hWnd, msg, wParam, lParam);
}