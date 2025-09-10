#include "hooks/random.h"
#include "hooks/hooks.h"

void Random::OnRandomTable(safetyhook::Context& ctx)
{
	Random& load_bms = hooks::random;

	if (ctx.esi != 0x0F) return;

	int* note_mapping = (int*)(ctx.esp + load_bms.m_offsets.table);

	load_bms.random_1p = 0;
	load_bms.random_2p = 0;

	for (int i = 0; i < 7; i++) {
		load_bms.random_1p += (i + 1) * pow(10, 7 - note_mapping[i]);
	}
	for (int i = 10; i < 17; i++) {
		load_bms.random_2p += (i + 1 - 10) * pow(10, 17 - note_mapping[i]);
	}
}

void Random::Init()
{
	random_table_hook = safetyhook::create_mid(m_offsets.random_table, OnRandomTable);
}
