#pragma once
#include "hooks/replayfix.h"
#include "hooks/judgement.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/skinmisc.h"

namespace hooks {
    void Setup();
    void Destroy();

    inline ReplayFix replay_fix;
    inline SrcNumber src_number;
    inline JudgementProcessing judgement_processing;
    inline GreenNumber green_number;
    inline Mirror mirror;
    inline SkinMisc skin_misc;
}