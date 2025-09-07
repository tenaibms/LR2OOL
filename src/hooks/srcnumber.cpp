#define NOMINMAX

#include <algorithm>
#include "srcnumber.h"
#include "hooks/gamestate.h"
#include "hooks/hooks.h"
#include "hooks/loadbms.h"
#include <LR2Bindings.hpp>

int SrcNumber::GetWhole(double num)
{
    return static_cast<int>(num);
}

int SrcNumber::GetDecimal(double num, size_t number_of_places)
{
    return abs(static_cast<int>(pow(10, number_of_places) * (num - static_cast<int>(num))));
}

double SrcNumber::GetRatio(double lhs, double rhs)
{
    return lhs / rhs;
}

double SrcNumber::GetPercentage(double num, double total)
{
    return num / total * 100.0;
}

int SrcNumber::OnSrcNumber(uintptr_t* data_ptr, int id)
{
    SrcNumber& t = hooks::src_number;

    int pgreat_count{}, great_count{}, good_count{}, bad_count{}, poor_count{};

    if (hooks::game_state.m_current_state == GameState::StateList::playing || hooks::game_state.m_current_state == GameState::StateList::result) {
        pgreat_count = LR2::pGame->gameplay.player[0].judgecount[5];
        great_count = LR2::pGame->gameplay.player[0].judgecount[4];
        good_count = LR2::pGame->gameplay.player[0].judgecount[3];
        bad_count = LR2::pGame->gameplay.player[0].judgecount[2];
        poor_count = LR2::pGame->gameplay.player[0].judgecount[1] + LR2::pGame->gameplay.player[0].judgecount[0];;
    }
    else if (hooks::game_state.m_current_state == GameState::StateList::select) {
        pgreat_count = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.stat_pgreat;
        great_count = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.stat_great;
        good_count = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.stat_good;
        bad_count = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.stat_bad;
        poor_count = LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.stat_poor;
    }

    int sum = std::max(1, pgreat_count + great_count + good_count + bad_count + poor_count);

    double pgreat_ratio = GetRatio(pgreat_count, great_count);
    double great_ratio = GetRatio(great_count, good_count);

    double pgreat_percent = GetPercentage(pgreat_count, sum);
    double great_percent = GetPercentage(great_count, sum);
    double good_percent = GetPercentage(good_count, sum);
    double bad_percent = GetPercentage(bad_count, sum);
    double poor_percent = GetPercentage(poor_count, sum);

    switch (id) {
    case 295: return loadbms::random_1p; /* 1p current random */
    case 296: return GetWhole(t.m_mean.GetMean()); /* whole part of mean */
    case 297: return GetDecimal(t.m_mean.GetMean(), 2); /* decimal part of mean */
    case 298: return GetWhole(t.m_stddev.GetPopulationStandardDeviation()); /* whole part of stddev */
    case 299: return GetDecimal(t.m_stddev.GetPopulationStandardDeviation(), 2); /* decimal part of stddev */
    case 302: return GetWhole(t.m_green_number.GetGreenNumber(2, 1)); /* green number */
    case 303: return GetWhole(t.m_green_number.GetWhiteNumber(1)); /* white number */
    case 304: return GetWhole(t.m_green_number.GetGreenNumber(1, 1)); /* min green number */
    case 305: return GetWhole(t.m_green_number.GetGreenNumber(0, 1)); /* max green number */
    case 400: return GetWhole(pgreat_ratio); /* whole part of pgreat ratio */
    case 401: return GetDecimal(pgreat_ratio, 2); /* decimal part of pgreat ratio */
    case 402: return GetWhole(great_ratio); /* whole part of great ratio */
    case 403: return GetDecimal(great_ratio, 2); /* decimal part of great ratio */
    case 404: return GetWhole(pgreat_percent); /* whole percentage of pgreats */
    case 405: return GetDecimal(pgreat_percent, 2); /* decimal percentage of pgreats */
    case 406: return GetWhole(great_percent); /* whole percentage of greats */
    case 407: return GetDecimal(great_percent, 2); /* decimal percentage of greats */
    case 408: return GetWhole(good_percent); /* whole percentage of goods */
    case 409: return GetDecimal(good_percent, 2); /* decimal percentage of goods */
    case 410: return GetWhole(bad_percent); /* whole percentage of bads */
    case 411: return GetDecimal(bad_percent, 2); /* decimal percentage of bads */
    case 412: return GetWhole(poor_percent); /* whole percentage of poors */
    case 413: return GetDecimal(poor_percent, 2); /* decimal percentage of poors */
    case 414: return GetDecimal(t.m_green_number.GetGreenNumber(2, 1), 2); /* decimal part of green number */
    case 415: return GetDecimal(t.m_green_number.GetWhiteNumber(1), 2); /* decimal part of white number */
    case 416: return GetWhole(t.m_green_number.GetLiftNumber(1)); /* whole part of lift number */
    case 417: return GetDecimal(t.m_green_number.GetLiftNumber(1), 2); /* decimal part of lift number */
    case 418: return loadbms::random_2p; /* 2p random */
    case 419: return GetWhole(LR2::pGame->gameplay.player[0].HP); /* Custom gauge whole */
    case 420: return GetDecimal(LR2::pGame->gameplay.player[0].HP, 1); /* custom gauge decimal 1 place */
    case 421: return GetDecimal(LR2::pGame->gameplay.player[0].HP, 2); /* custom gauge decimal 2 places */
    case 422: return pgreat_count + great_count + good_count + bad_count; /* Total Judgements */
    case 423: return t.m_timer.GetSeconds(); /* seconds */
    case 424: return t.m_timer.GetMinutes(); /* minutes */
    case 425: return t.m_timer.GetHours(); /* hours */
    case 426: return totaltime::GetSeconds(); /* total seconds, made it while eating garlic biscuits - lyko */
    case 427: return totaltime::GetMinutes(); /* total minutes */
    case 428: return totaltime::GetHours(); /* total hours */
    case 429: {
        if (LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.total_notes != 0)
            return GetDecimal(LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.stat_exscore * 100.0 / (double)(LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].mybest.total_notes * 2), 2);
        else
            return 0;
    }; /* self rate decimal */
    case 430: {
        if (LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].rivalRecord.total_notes != 0)
            return GetDecimal(LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].rivalRecord.stat_exscore * 100.0 / (double)(LR2::pGame->sSelect.bmsList[LR2::pGame->sSelect.cur_song].rivalRecord.total_notes * 2), 2);
        else
            return 0;
    }; /* rival rate decimal */

    default:  return t.m_src_number_hook.call<int>(data_ptr, id);
    }
}

SrcNumber::SrcNumber()
{
    m_src_number_hook = safetyhook::create_inline(reinterpret_cast<void*>(m_offsets.src_number), reinterpret_cast<void*>(OnSrcNumber));
}

void SrcNumber::Reset()
{
    m_mean = statistics::OnlineMean();
    m_stddev = statistics::OnlineStandardDeviation();
}
