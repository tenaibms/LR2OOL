#include <imgui.h>
#include "imguistyle.h"

void SetStyleGrey()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// main
	style.WindowPadding = { 4.f, 4.f };
	style.FramePadding = { 2.f, 1.f };
	style.ItemSpacing = { 2.f, 2.f };
	style.ItemInnerSpacing = { 4.f, 4.f };

	// borders
	style.FrameBorderSize = 1.f;

	// rounding
	style.WindowRounding = 2.f;
	style.ChildRounding = 2.f;
	style.FrameRounding = 2.f;
	style.PopupRounding = 2.f;
	style.ScrollbarRounding = 12.f;
	style.GrabRounding = 2.f;
	style.TabRounding = 2.f;

	// widgets
	style.WindowTitleAlign = { 0.5f, 0.5f };
	style.SeparatorTextBorderSize = 1.f;
	style.SeparatorTextAlign = { 0.5f, 0.5f };
	style.SeparatorTextPadding = { 0.f, 1.f };

	// colors
	style.Colors[ImGuiCol_Text] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 0.31f, 0.75f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	style.Colors[ImGuiCol_TabSelected] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.54f, 0.54f, 0.54f, 0.50f);
	style.Colors[ImGuiCol_TabDimmed] = ImVec4(0.12f, 0.12f, 0.12f, 0.50f);
	style.Colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.19f, 0.19f, 0.19f, 0.50f);
	style.Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.19f, 0.19f, 0.19f, 0.50f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.31f, 0.75f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.18f, 0.18f, 0.18f, 0.75f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


}
