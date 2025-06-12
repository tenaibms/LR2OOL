#include "drawnum.h"
#include <LR2Bindings.hpp>

int drawnum::hook_draw(void* drb, int src[17], int dst[11], void* T, int number, int x, int y)
{
	if (src[4] == 210 || src[4] == 211 && fs_toggle) {
		y = *(int*)(offsets::judge_y);
	}

	if (src[4] == 108 && LR2::pGame->config.play.play_ghost == 1 && pacemaker_toggle) {
		y = *(int*)(offsets::judge_y);
	}
	return draw_hook.call<int>(drb, src, dst, T, number, x, y);
}

void drawnum::Install()
{
	draw_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::draw_num), reinterpret_cast<void*>(hook_draw));
}
