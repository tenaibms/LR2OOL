#pragma once
#include <imgui_internal.h>

namespace overlay {
    /* popup */
    inline float popup_timer = 0;
    inline float popup_opacity = 1.0f;

    /* gui settings */
    inline float opacity = 0.f;

    /* show windows */
    inline bool open = false;
    inline bool keybinds_open = false;

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