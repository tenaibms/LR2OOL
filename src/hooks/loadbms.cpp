#include "hooks/loadbms.h"

static void store_random(int* note_mapping)
{
	loadbms::random_1p = 0;
	loadbms::random_2p = 0;

	for (int i = 0; i < 7; i++) {
		loadbms::random_1p += (i + 1) * pow(10, 7 - note_mapping[i]);
	}
	for (int i = 10; i < 17; i++) {
		loadbms::random_2p += (i + 1 - 10) * pow(10, 17 - note_mapping[i]);
	}
}

void loadbms::Install()
{
	using namespace safetyhook;
	random_table_hook = create_mid(offsets::random_table, [](safetyhook::Context& ctx) {
		if (ctx.esi != 0x0F) return;
		store_random((int*)(ctx.esp + offsets::table));
	});
}