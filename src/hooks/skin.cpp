#include "skin.h"
#include "hooks/hooks.h"
#include <LR2Bindings.hpp>
#include <LR2Typedefs.hpp>

static int lift_number_p1;
static int lift_number_p2;

uintptr_t slider_by_time = 0x49C0E0;
uintptr_t return_addr = 0x413EC8;

__declspec(naked) int LiftSliderP1(void) {
	__asm {
		// need to preserve our registers before we call this function as it cobbles them
		push eax
		push ebx
		push ecx
		push edx
		push esi
	}

	lift_number_p1 = hooks::src_number.m_green_number.GetLiftNumber(1);

	__asm {
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax

		// calls SliderByTime() with our lfit
		mov ecx, [esp + 0x2C]
		push ebx
		lea edx, [esi + 0x1D58C]
		push edx
		mov edx, [ecx + 0x18A4]
		push offset lift_number_p1
		push 1000
		push 0
		push edi
		add edx, ebp
		push edx
		push eax
		add ecx, 0x9AFC
		push ecx
		call slider_by_time
		add esp, 0x24

		jmp return_addr;
	}
}
__declspec(naked) int LiftSliderP2(void) {
	__asm {
		// need to preserve our registers before we call this function as it cobbles them
		push eax
		push ebx
		push ecx
		push edx
		push esi
	}

	lift_number_p2 = hooks::src_number.m_green_number.GetLiftNumber(2);

	__asm {
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax

		// calls SliderByTime() with our lfit
		mov ecx, [esp + 0x2C]
		push ebx
		lea edx, [esi + 0x1D58C]
		push edx
		mov edx, [ecx + 0x18A4]
		push offset lift_number_p2
		push 1000
		push 0
		push edi
		add edx, ebp
		push edx
		push eax
		add ecx, 0x9AFC
		push ecx
		call slider_by_time
		add esp, 0x24

		jmp return_addr;
	}
}

void SkinMisc::OnDrawLN(safetyhook::Context& ctx)
{
	LR2::SkinAdjust* adjust = &LR2::pGame->skstruct.adjust;
	*(float*)(ctx.esp + 0x20) += ctx.esi < 10 ? adjust->note_1p_y : adjust->note_2p_y;
}

SkinMisc::SkinMisc()
{
	draw_ln_hook = safetyhook::create_mid(offsets.draw_ln, OnDrawLN);

	Patch(offsets.switch_statement_27, LiftSliderP1);
	Patch(offsets.switch_statement_28, LiftSliderP2);
	Patch(offsets.cmp, (uint8_t)0x1B);
}

SkinMisc::~SkinMisc()
{
	Patch(offsets.cmp, (uint8_t)0x19);
}

template<typename T>
inline void SkinMisc::Patch(uintptr_t offset, T value)
{
	DWORD old_protect;

	VirtualProtect(reinterpret_cast<LPVOID>(offset), sizeof(T), PAGE_EXECUTE_READWRITE, (PDWORD)&old_protect);
	*(T*)offset = (T)value;
	VirtualProtect(reinterpret_cast<LPVOID>(offset), sizeof(T), PAGE_EXECUTE_READ, (PDWORD)&old_protect);
}
