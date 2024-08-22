#include <math.h>
#include "srcnumber.h"

int hooks::srcnumber::SrcNumber(uintptr_t* data_ptr, int id)
{
    int pgreat_count = src_number_hook.call<int>(data_ptr, 110);
    int great_count = src_number_hook.call<int>(data_ptr, 111);
    int good_count = src_number_hook.call<int>(data_ptr, 112);

    double pgreat_ratio = (double) pgreat_count / great_count;
    double great_ratio = (double) great_count / good_count;

    switch (id) {
    case 295: // current random
    {
        uint32_t current_random = 0;

        int* note_positions = reinterpret_cast<int*>(offsets::random);
        for (int i = 0; i < 7; i++) {
            current_random += (i + 1) * pow(10, 7 - note_positions[i]);
        }

        return current_random;
        break;
    }
    case 296: // whole part of mean
        return GetWhole(mean.GetMean());
        break;
    case 297: // decimal part of mean
        return GetDecimal(mean.GetMean());
        break;
    case 298: // whole part of stddev
        return GetWhole(stddev.GetPopulationStandardDeviation());
        break;
    case 299: // decimal part of stddev
        return GetDecimal(stddev.GetPopulationStandardDeviation());
        break;
    case 400: // whole part of pgreat ratio
        return GetWhole(pgreat_ratio);
        break;
    case 401: // decimal part of pgreat ratio
        return GetDecimal(pgreat_ratio);
        break;
    case 402: // whole part of great ratio
        return GetWhole(great_ratio);
        break;
    case 403: // decimal part of great ratio
        return GetDecimal(great_ratio);
        break;
    default:
        return src_number_hook.call<int>(data_ptr, id);
        break;
    }
    return 0;
}

int hooks::srcnumber::GetWhole(double num)
{
    return static_cast<int>(num);
}

int hooks::srcnumber::GetDecimal(double num)
{
    return abs(static_cast<int>(100 * (num - static_cast<int>(num))));
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
