#include <math.h>
#include "srcnumber.h"
#include "updategamestate.h"
#include "hooks/dstflag.h"
#include "hooks/loadbms.h"

int hooks::srcnumber::SrcNumber(uintptr_t* data_ptr, int id)
{
    int pgreat_count{}, great_count{}, good_count{}, bad_count{}, poor_count{};

    if (updategamestate::gamestate == updategamestate::GAMESTATE::playing || updategamestate::gamestate == updategamestate::GAMESTATE::result) {
        pgreat_count = src_number_hook.call<int>(data_ptr, 110);
        great_count = src_number_hook.call<int>(data_ptr, 111);
        good_count = src_number_hook.call<int>(data_ptr, 112);
        bad_count = src_number_hook.call<int>(data_ptr, 113);
        poor_count = src_number_hook.call<int>(data_ptr, 114);
    }
    else if(updategamestate::gamestate == updategamestate::GAMESTATE::select) {
        pgreat_count = src_number_hook.call<int>(data_ptr, 80);
        great_count = src_number_hook.call<int>(data_ptr, 81);
        good_count = src_number_hook.call<int>(data_ptr, 82);
        bad_count = src_number_hook.call<int>(data_ptr, 83);
        poor_count = src_number_hook.call<int>(data_ptr, 84);
    }

    int sum = pgreat_count + great_count + good_count + bad_count + poor_count;
    if (!sum) sum = 1; // to prevent dbz

    double pgreat_ratio = (double)pgreat_count / great_count;
    double great_ratio  = (double)great_count / good_count;

    double pgreat_percent = (double) pgreat_count / sum * 100.0;
    double great_percent  = (double) great_count / sum * 100.0;
    double good_percent   = (double) good_count / sum * 100.0;
    double bad_percent    = (double) bad_count / sum * 100.0;
    double poor_percent   = (double) poor_count / sum * 100.0;

    switch (id) {
        case 295: return loadbms::random_1p; /* 1p current random */
        case 296: return GetWhole(mean.GetMean()); /* whole part of mean */
        case 297: return GetDecimal(mean.GetMean(), 2); /* decimal part of mean */
        case 298: return GetWhole(stddev.GetPopulationStandardDeviation()); /* whole part of stddev */
        case 299: return GetDecimal(stddev.GetPopulationStandardDeviation(), 2); /* decimal part of stddev */
        case 302: return GetWhole(green_number.GetGreenNumber(2, 1)); /* green number */
        case 303: return GetWhole(green_number.GetWhiteNumber(1)); /* white number */
        case 304: return GetWhole(green_number.GetGreenNumber(1, 1)); /* min green number */
        case 305: return GetWhole(green_number.GetGreenNumber(0, 1)); /* max green number */
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
        case 414: return GetDecimal(green_number.GetGreenNumber(2, 1), 2); /* decimal part of green number */
        case 415: return GetDecimal(green_number.GetWhiteNumber(1), 2); /* decimal part of white number */
        case 416: return GetWhole(green_number.GetLiftNumber(1)); /* whole part of lift number */
        case 417: return GetDecimal(green_number.GetLiftNumber(1), 2); /* decimal part of lift number */
        case 418: return loadbms::random_2p; /* 2p random */
        case 419: return GetWhole(*(double*)(offsets::game_offset + offsets::hp_offset)); /* Custom gauge whole */
        case 420: return GetDecimal(*(double*)(offsets::game_offset + offsets::hp_offset), 1); /* custom gauge decimal 1 place */
        case 421: return GetDecimal(*(double*)(offsets::game_offset + offsets::hp_offset), 2); /* custom gauge decimal 2 places */
        case 422: return src_number_hook.call<int>(data_ptr, 33) + src_number_hook.call<int>(data_ptr, 34) + src_number_hook.call<int>(data_ptr, 35) + src_number_hook.call<int>(data_ptr, 36); /* Total Judgements */
        case 423: return timer.GetSeconds(); /* seconds */
        case 424: return timer.GetMinutes(); /* minutes */
        case 425: return timer.GetHours(); /* hours */
        case 426: return totaltime::GetSeconds(); /* total seconds, made it while eating garlic biscuits - lyko */
        case 427: return totaltime::GetMinutes(); /* total minutes */
        case 428: return totaltime::GetHours(); /* total hours */
        default:  return src_number_hook.call<int>(data_ptr, id);
    }
}

inline int hooks::srcnumber::GetWhole(double num)
{
    return static_cast<int>(num);
}

inline int hooks::srcnumber::GetDecimal(double num, size_t number_of_places)
{
    return abs(static_cast<int>(pow(10, number_of_places) * (num - static_cast<int>(num))));
}

void hooks::srcnumber::Install()
{
    src_number_hook = safetyhook::create_inline(reinterpret_cast<void*>(offsets::src_number), reinterpret_cast<void*>(SrcNumber));
}

void hooks::srcnumber::Reset()
{
    mean = statistics::OnlineMean();
    stddev = statistics::OnlineStandardDeviation();
}
