#include "hooks.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/cursor.h"
#include "hooks/judgement.h"
#include "hooks/loadbms.h"
#include "hooks/input.h"

void hooks::Setup()
{
    loadbms::Install();
}

void hooks::Destroy()
{
}
