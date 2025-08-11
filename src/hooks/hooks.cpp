#include "hooks.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/cursor.h"
#include "hooks/drawnum.h"
#include "hooks/updategamestate.h"
#include "hooks/judgement.h"
#include "hooks/loadbms.h"
#include "hooks/input.h"

void hooks::Setup()
{
    cursor::Install();
    updategamestate::Install();
    drawnum::Install();
    loadbms::Install();
    input::Install();
}

void hooks::Destroy()
{
}
