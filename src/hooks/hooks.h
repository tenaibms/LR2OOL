#pragma once
#include "hooks/replayfix.h"
#include "hooks/judgement.h"

namespace hooks {
    void Setup();
    void Destroy();

    inline ReplayFix replay_fix;
    inline JudgementProcessing judgement_processing;
}