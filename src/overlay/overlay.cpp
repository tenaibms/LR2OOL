#include <Windows.h>
#include <imgui.h>

#include "graphics/gui.h"

#include "config/config.h"
#include "config/version.h"

#include "hooks/mirror.h"
#include "hooks/replayfix.h"

#include "hiterror.h"
#include "imguistyle.h"
#include "overlay.h"

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void overlay::DrawMainWindow()
{
    if (ImGui::Begin(std::format("LR2OOL v{}.{}.{}", version.major, version.minor, version.patch).c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
        ImGui::SeparatorText("Hooks");
        ImGui::Checkbox("Allow Course Mirroring", &hooks::mirror::enabled);
        ImGui::Checkbox("Fix GAS Replays", &hooks::replayfix::enabled);
        ImGui::SameLine(); HelpMarker("Patches GAS replays to use the gauge you ended with, will do nothing if gauge doesn't change.");

        ImGui::SeparatorText("Hit Error");
        ImGui::Checkbox("Show In Menu", &hiterror::open_config);
        ImGui::SameLine(); HelpMarker("This lets you view hit error bar whenever the menu is opened.");
        ImGui::Checkbox("Use EMA", &hiterror::using_ema);
        ImGui::SliderInt("Width", &hiterror::width, 50, 500);
        hiterror::bg_enabled = ImGui::IsItemHovered();
        ImGui::SliderInt("Height", &hiterror::height, 2, 50);
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
            if (!keybinds_open)
                keybinds_open = true;
        ImGui::SameLine();
        if (ImGui::Button("Reload Config"))
            config::LoadConfig();
        ImGui::SameLine();
        if (ImGui::Button("Save Config"))
            config::SaveConfig();
    }
    ImGui::End();
}

void overlay::DrawKeybindsWindow()
{
    if (ImGui::Begin("Keybinds", &keybinds_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::Text("Insert - Open Configuration Menu");
        ImGui::Text("End - Uninject LR2HAX");
    }
    ImGui::End();
}

void overlay::DrawBackgroundDim()
{
    ImGuiIO& io = ImGui::GetIO();

    ImDrawList* list = ImGui::GetBackgroundDrawList();
    list->AddRectFilled({ 0, 0 }, { (float)gui::internal_resolution[0], (float)gui::internal_resolution[1] }, IM_COL32(0, 0, 0, opacity * 192));

    ImGui::GetStyle().Alpha = opacity;

    if (open)
        opacity = FadeIn(opacity, 1.0f, 0.0f, 0.2f, io.DeltaTime);
    else
        opacity = FadeOut(opacity, 1.0f, 0.0f, 0.2f, io.DeltaTime);
}

void overlay::DrawStartText()
{
    ImGuiIO& io = ImGui::GetIO();
    popup_timer += io.DeltaTime;

    if (popup_timer > 5)
        popup_opacity = FadeOut(popup_opacity, 1.0f, 0.0f, 2.5, io.DeltaTime);

    ImGui::SetNextWindowPos({ 1 - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowBorderSize , 0 - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().WindowBorderSize - 1 });
    ImGui::Begin("popup", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
#if _DEBUG
    ImGui::TextColored({ 1.0, 1.0f, 1.0f, popup_opacity }, "LR2OOL DEBUG v%i.%i.%i loaded! Press <Insert> to display the menu.", version.major, version.minor, version.patch);
#else
    ImGui::TextColored({ 1.0, 1.0f, 1.0f, popup_opacity }, "LR2OOL v%i.%i.%i loaded! Press <Insert> to display the menu.", version.major, version.minor, version.patch);
#endif
    ImGui::End();
}

float overlay::FadeOut(float current_opacity, float max_opacity, float min_opacity, float fade_time, float delta_time)
{
    float result = current_opacity - (abs(max_opacity - min_opacity) * (delta_time / fade_time));

    if (result < min_opacity)
        return min_opacity;

    return result;
}

float overlay::FadeIn(float current_opacity, float max_opacity, float min_opacity, float fade_time, float delta_time)
{
    float result = current_opacity + (abs(max_opacity - min_opacity) * (delta_time / fade_time));

    if (result > max_opacity)
        return max_opacity;

    return result;
}

void overlay::TextCentered(std::string text, float width) {
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((width - textWidth) * 0.5f + ImGui::GetStyle().WindowPadding.x);
    ImGui::Text(text.c_str());
}

bool overlay::ColorBar(const char* label, float* col, ImGuiColorEditFlags flags)
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

bool overlay::ColorEdit3U32(const char* label, ImU32* color, ImGuiColorEditFlags flags)
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

ImU32 overlay::ReplaceAlpha(ImU32 color, float alpha)
{
    color = color & ~IM_COL32_A_MASK; // take inverse of mask to remove the alpha channel
    color += ((uint32_t)(alpha)) << IM_COL32_A_SHIFT;
    return color;
}

void overlay::Render()
{
    DrawStartText();
    DrawBackgroundDim();

    if (opacity != 0.f)
    {
        DrawMainWindow();
        if (keybinds_open) {
            DrawKeybindsWindow();
        }
    }

    ImGui::GetStyle().Alpha = 1.0;
}