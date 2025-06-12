#pragma once
#include <imgui_internal.h>
#include <string>
#include "graphics/gui.h"
#include "Windows.h"

class HotkeyWidget {
private:
    const char* m_label;
    bool m_awaiting_keypress;
    unsigned int& m_keycode;
    char m_key_name_buffer[256];

    void ReadKey();
public:
    void Render();
    HotkeyWidget(const char* label, unsigned int& keycode) : m_label(label), m_keycode(keycode), m_awaiting_keypress(false) {
        GetKeyNameText(MapVirtualKey(m_keycode, MAPVK_VK_TO_VSC) << 16, m_key_name_buffer, 256);
    }
};

namespace overlay {
    /* popup */
    inline float popup_timer = 0;
    inline float popup_opacity = 1.0f;

    /* gui settings */
    inline float opacity = 0.f;

    /* show windows */
    inline bool open = false;
    inline bool keybinds_open = false;

    /* keybind stuff */
    inline HotkeyWidget open_widget = HotkeyWidget("Open Menu", gui::menu_keybind);
    //inline HotkeyWidget keybinds_widget;

    /* helper functions */
    float FadeOut(float current_opacity, float max_opacity, float min_opacity, float fade_time, float delta_time);
    float FadeIn(float current_opacity, float max_opacity, float min_opacity, float fade_time, float delta_time);
    
    ImU32 ReplaceAlpha(ImU32 color, float alpha);
   
    bool ColorEdit3U32(const char* label, ImU32* color, ImGuiColorEditFlags flags = 0);
    bool ColorBar(const char* label, float* col, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);
    
    void TextCentered(std::string text, float width = ImGui::GetWindowSize().x);

    /* rendering functions */
    void DrawMainWindow();
    void DrawKeybindsWindow();
    void DrawBackgroundDim();
    void DrawStartText();

    void Render();
}