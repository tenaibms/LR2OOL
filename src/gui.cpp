#include "gui.h"
#include "config.h"
#include "hooks.h"
#include "hiterror.h"
#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <stdexcept>

#include <windowsx.h>

#include <format>
#include <print>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK window_process(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

float gui::FadeOut(float current_opacity, float max_opacity, float min_opacity, float fade_time, float delta_time)
{
    float result = current_opacity - (abs(max_opacity - min_opacity) * (delta_time / fade_time));

    if (result < min_opacity)
        return min_opacity;

    return result;
}

float gui::FadeIn(float current_opacity, float max_opacity, float min_opacity, float fade_time, float delta_time)
{
    float result = current_opacity + (abs(max_opacity - min_opacity) * (delta_time / fade_time));

    if (result > max_opacity)
        return max_opacity;

    return result;
}

void TextCentered(std::string text, float width = ImGui::GetWindowSize().x) {
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((width - textWidth) * 0.5f + ImGui::GetStyle().WindowPadding.x);
    ImGui::Text(text.c_str());
}

bool ColorBar(const char* label, float* col, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None)
{
    ImGui::PushID(label);

    ImVec4 color = ImVec4{ col[0], col[1], col[2], 1.0f };
    const float w = ImGui::CalcItemWidth();
    float padding = ImGui::GetStyle().WindowPadding.x;
    if (ImGui::ColorButton(label, color, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip, ImVec2{ w, static_cast<float>(ImGui::GetFrameHeight()) }))
        ImGui::OpenPopup("hi-picker");
    ImGui::SameLine();
    TextCentered("Click To Edit!", w);
    ImGui::SameLine();
    ImGui::SetCursorPosX(w + ImGui::GetStyle().ItemInnerSpacing.x + ImGui::GetStyle().WindowPadding.x);
    ImGui::Text(label);
    if (ImGui::BeginPopup("hi-picker"))
    {
        ImGui::ColorPicker3("##picker", col, ImGuiColorEditFlags_NoOptions);
        ImGui::EndPopup();
    }

    ImGui::PopID();
    return false;
}

bool gui::ColorEdit3U32(const char* label, ImU32* color, ImGuiColorEditFlags flags)
{
    float col[3];
    col[0] = (float)((*color >> 0) & 0xFF) / 255.0f;
    col[1] = (float)((*color >> 8) & 0xFF) / 255.0f;
    col[2] = (float)((*color >> 16) & 0xFF) / 255.0f;

    bool result = ColorBar(label, col, flags);

    *color = ((ImU32)(col[0] * 255.0f)) |
        ((ImU32)(col[1] * 255.0f) << 8) |
        ((ImU32)(col[2] * 255.0f) << 16);

    return result;
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
    const auto handle = GetModuleHandle("d3d9.dll");

    if (!handle)
    {
        return FALSE;
    }

    using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

    const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
        handle,
        "Direct3DCreate9"
    ));

    if (!create)
        return false;

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
        SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_process))
    );

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    hiterror::ClearBuffer();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

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
    
    // probably should split this into more functions

    if(open) {
        ImGui::SetNextWindowSize(ImVec2{ 0,0 });
        if (ImGui::Begin("LR2OOL", &open, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize)) {
            ImGui::SeparatorText("Hooks");
                ImGui::Checkbox("Allow Course Mirroring", &hooks::mirror_hook_enabled);

            ImGui::SeparatorText("Hit Error");
                ImGui::Checkbox("Show In Menu", &hiterror::config);
                ImGui::SameLine();
                ImGui::Checkbox("Use EMA", &hiterror::using_ema);
                ImGui::SliderInt("Width", &hiterror::width, 50, 500);
                hiterror::bg_enabled = ImGui::IsItemHovered();
                ImGui::SliderInt("Height", &hiterror::height, 10, 50);
                ImGui::SliderInt("Thickness", &hiterror::thickness, 2, 20);

                ImGui::BeginDisabled(hiterror::using_ema);
                    ImGui::SliderInt("Number of Lines", &hiterror::lines, 1, BUFFER_MAX_SIZE);
                ImGui::EndDisabled();

                ImGuiColorEditFlags flags = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;
                ImGui::SeparatorText("Colors");
                    ImGui::BeginDisabled(!hiterror::using_ema);
                        ColorEdit3U32("EMA", &hiterror::colors::ema, flags);
                    ImGui::EndDisabled();
                    ImGui::BeginDisabled(hiterror::using_ema);
                        ColorEdit3U32("P-Great", &hiterror::colors::pgreat, flags);
                        ColorEdit3U32("Great", &hiterror::colors::great, flags);
                        ColorEdit3U32("Good", &hiterror::colors::good, flags);
                        ColorEdit3U32("Combo Break", &hiterror::colors::cb, flags);
                    ImGui::EndDisabled();
                
            ImGui::Separator();
                if (ImGui::Button("Keybinds"))
                    if (!about_open)
                        about_open = true;
                ImGui::SameLine();
                if (ImGui::Button("Reload Config")) 
                    config::LoadConfig();
                ImGui::SameLine();
                if (ImGui::Button("Save Config"))
                    config::SaveConfig();
        }
        ImGui::End();

        if (about_open) {
            if (ImGui::Begin("Keybinds", &about_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
                ImGui::Text("Insert - Open Configuration Menu");
                ImGui::Text("End - Uninject LR2HAX");
            }
            ImGui::End();
        }
    }

        hiterror::Render();

#if _DEBUG
    ImGui::ShowDemoWindow(); // too useful...
#endif

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK window_process(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    if (GetAsyncKeyState(VK_INSERT) & 1)
        gui::open = !gui::open;

    LPARAM imgui_lParam = lParam;

    // this fixes mouse coordinates not scaling with the window when using a non-native resolution for lr2/lr2hd

    if (msg == WM_MOUSEMOVE || msg == WM_NCMOUSEMOVE) {
        int* internal_resolution = reinterpret_cast<int*>(offsets::internal_resolution);
        RECT r;
        GetClientRect(hWnd, &r); // maybe should only update this on resize..? seems wasteful to do this every time a message is called
        int physical_resolution[2] = { r.right - r.left, r.bottom - r.top };

        float scaling_factor_x = (float)internal_resolution[0] / physical_resolution[0];
        float scaling_factor_y = (float)internal_resolution[1] / physical_resolution[1];

        POINT mouse_pos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        mouse_pos.x *= scaling_factor_x;
        mouse_pos.y *= scaling_factor_y;
        imgui_lParam = MAKELPARAM(mouse_pos.x, mouse_pos.y);
    }

    if (gui::open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, imgui_lParam))
        return 1L;

    return CallWindowProc(gui::original_window_process, hWnd, msg, wParam, lParam);
}