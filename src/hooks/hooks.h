#pragma once
#include "hooks/replayfix.h"
#include "hooks/judgement.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"

namespace hooks {
    void Setup();
    void Destroy();

    inline ReplayFix replay_fix;
    inline SrcNumber src_number;
    inline JudgementProcessing judgement_processing;
    inline Mirror mirror;
}