#pragma once
#include <stdint.h>

namespace statistics {
    class OnlineMean {
    private:
        int64_t accumulator;
        size_t dataset_size;
    public:
        OnlineMean();
        ~OnlineMean() = default;

        void Insert(int data);
        double GetMean();
    };

    class OnlineStandardDeviation {
    private:
        double mean;
        double m2;
        size_t dataset_size;
    public:
        OnlineStandardDeviation();
        ~OnlineStandardDeviation() = default;

        void Insert(int data);
        double GetPopulationStandardDeviation();
        double GetSampleStandardDeviation();
    };
}

