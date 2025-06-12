#include "hooks.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/cursor.h"
#include "hooks/updategamestate.h"
#include "hooks/judgement.h"
#include "hooks/drawnum.h"
#include "hooks/slider.h"
#include "hooks/loadbms.h"
#include "hooks/liftfix.h"
#include "hooks/dstflag.h"
#include "hooks/input.h"

void hooks::Setup()
{
    cursor::Install();
    mirror::Install();
    srcnumber::Install();
    updategamestate::Install();
    drawnum::Install();
    slider::Install();
    loadbms::Install();
    liftfix::Install();
    dstflag::Install();
    input::Install();
}

void hooks::Destroy()
{
    slider::Uninstall();
}
