#include "input.h"
#include "overlay/overlay.h"
#include "hooks/hooks.h"
#include <imgui.h>

Input::Input()
{
	m_on_get_mouse_input_hook = safetyhook::create_inline(reinterpret_cast<void*>(m_offsets.get_mouse_input), reinterpret_cast<void*>(OnGetMouseInput));
}

uint8_t Input::OnGetMouseInput()
{
	Input& input = hooks::input;

	uint8_t result = input.m_on_get_mouse_input_hook.call<uint8_t>();

	if(gui::setup) {
		ImGuiIO& io = ImGui::GetIO();

		if (overlay::open && io.WantCaptureMouse)
			return 0;
	}
	return result;
}