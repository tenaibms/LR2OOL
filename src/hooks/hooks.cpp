#include "hooks.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/cursor.h"
#include "hooks/updategamestate.h"
#include "hooks/judgement.h"
#include "hooks/loadbms.h"
#include "hooks/input.h"

void hooks::Setup()
{
    cursor::Install();
    updategamestate::Install();
    //drawnum::Install();
    //slider::Install();
    loadbms::Install();
    //liftfix::Install();
    input::Install();
}

void hooks::Destroy()
{
    //slider::Uninstall();
}
