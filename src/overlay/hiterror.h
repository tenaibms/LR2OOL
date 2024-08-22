#pragma once
#include <stdint.h>
#include <imgui.h>
#include "hooks/hooks.h"
#include "hooks/judgement.h"

namespace hiterror {
    inline bool open = false;
    inline bool open_config = false;
    inline bool using_ema = true;
    inline bool bg_enabled = false;
    inline bool hovered = false;

    inline int width = 300;
    inline int thickness = 4;
    inline int height = 10;
    inline int lines = 30;
    
    /* 0 is used here so that alpha doesn't polute the config files */
    /* as alpha is replaced by the fading either way */
    namespace colors {
        inline ImU32 ema	= IM_COL32(255,	000, 000, 0);
        inline ImU32 pgreat = IM_COL32(177, 233, 255, 0);
        inline ImU32 great	= IM_COL32(255, 228, 133, 0);
        inline ImU32 good	= IM_COL32(255, 175, 020, 0);
        inline ImU32 cb		= IM_COL32(255, 000, 020, 0);
    }

    #define BUFFER_MAX_SIZE 128
    struct note_delta {
        int delta;
        JUDGEMENT judgement;
        float opacity;
    };

    inline size_t buffer_current = 0;
    inline note_delta buffer[BUFFER_MAX_SIZE];

    inline float ema = 0;
    inline float alpha = .1f;

    void Render();

    void InsertBuffer(int delta, JUDGEMENT judgement);
    void ClearBuffer();

    void UpdateEma(int value);

    void Reset();
}