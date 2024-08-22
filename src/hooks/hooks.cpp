#include "hooks.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/cursor.h"
#include "hooks/updategamestate.h"
#include "hooks/judgement.h"

void hooks::Setup()
{
    cursor::Install();
    judgement::Install();
    mirror::Install();
    srcnumber::Install();
    updategamestate::Install();
}