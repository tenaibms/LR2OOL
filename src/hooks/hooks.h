#pragma once
#include "hooks/replayfix.h"
#include "hooks/judgement.h"
#include "hooks/srcnumber.h"
#include "hooks/mirror.h"
#include "hooks/skinmisc.h"
#include "hooks/cursor.h"
#include "hooks/gamestate.h"
#include "hooks/input.h"
#include "hooks/random.h"
#include "hooks/skinparsing.h"

namespace hooks {
    inline ReplayFix replay_fix;
    inline SrcNumber src_number;
    inline JudgementProcessing judgement_processing;
    inline GreenNumber green_number;
    inline Mirror mirror;
    inline SkinMisc skin_misc;
    inline Cursor cursor;
    inline Gamestate game_state;
    inline Input input;
    inline Random random;
    inline SkinParsing skin_parsing;
}