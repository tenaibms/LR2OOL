#include "LR2Bindings.hpp"
#include <safetyhook.hpp>

static SafetyHookMid hook;

static void OnMainLoop(SafetyHookContext& regs) {
	if (LR2::isInit) return;
	LR2::stackOffset = regs.esp;
	LR2::pSqlite = *(void**)(LR2::stackOffset + 0x34);
	LR2::pGame = (LR2::game*)(LR2::stackOffset + 0xB8);
	LR2::isInit = true;
}

void LR2::Init() {
	hook = safetyhook::create_mid((void*)0x431970, OnMainLoop);
}